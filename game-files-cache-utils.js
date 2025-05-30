/**
 * @param {IDBOpenDBRequest} openRequest
 * @param {string} filePath
 * @param {ArrayBuffer} file
 * @returns {Promise<void>}
 */
export async function saveFileToIndexedDb(openRequest, filePath, file) {
    /** @type {IDBDatabase} */
    let db
    try {
        // This throws if the request is still pending
        db = openRequest.result
    } catch (error) {
        db = await new Promise(r => {
            openRequest.addEventListener(
                'success',
                () => r(openRequest.result),
                { once: true },
            );
        });
    }
    const transaction = db.transaction('thelongestyard', 'readwrite');
    const store = transaction.objectStore('thelongestyard');
    const blob = new Blob([file]);
    const _putRequest = store.put(blob, filePath);
    await new Promise((resolve, reject) => {
        // Waiting for `transaction.oncomplete`
        // instead of `putRequest.onsuccess`,
        // because if we go to another page too soon,
        // the file (especially pak0.pk3) might not get actually saved.
        transaction.oncomplete = () => {
            console.log(`${filePath} stored to indexedDB successfully!`);
            resolve();

            transaction.oncomplete = null;
            transaction.onerror = null;
        };
        transaction.onerror = (event) => {
            reject(`Error storing ${filePath} to indexedDB: ${event.target.error}`);

            transaction.oncomplete = null;
            transaction.onerror = null;
        };
    })
}

/**
 * @param {IDBOpenDBRequest} openRequest
 * @param {string} filePath
 * @returns {Promise<undefined | ArrayBuffer>}
 */
export function getFileFromIndexedDb(openRequest, filePath) {
    return new Promise(resolve_ => {
        const resolveAndCleanUp = (val) => {
            resolve_(val)
            openRequest.removeEventListener('error', onError);
            openRequest.removeEventListener('success', onSuccess);
        }

        const onError = () => {
            resolveAndCleanUp(undefined);
            console.error(
                `Error retrieving ${filePath} from indexedDB:`,
                openRequest.error.error
            );
        };

        let isPending;
        try {
            // This throws an error if it's still pending
            const _dummy = openRequest.error;
            isPending = false;
        } catch (err) {
            isPending = true;
        }

        if (!isPending && openRequest.error) {
            onError();
            return
        } else {
            openRequest.addEventListener('error', onError, { once: true });
        }

        const onSuccess = () => {
            /** @type {IDBDatabase} */
            const db = openRequest.result;
            const transaction = db.transaction('thelongestyard', 'readonly');
            const store = transaction.objectStore('thelongestyard');

            const getRequest = store.get(filePath);

            const resolveAndCleanUp2 = (val) => {
                resolveAndCleanUp(val);
                getRequest.onsuccess = null;
                getRequest.onerror = null;
            }

            getRequest.onsuccess = (event) => {
                /** @type {Blob | undefined} */
                const blob = event.target.result;

                if (!blob) {
                    resolveAndCleanUp2(undefined);
                    return;
                }

                blob.arrayBuffer().then(resolveAndCleanUp2)
            };

            getRequest.onerror = (event) => {
                resolveAndCleanUp2(undefined);
                console.error(
                    `Error retrieving ${filePath} from indexedDB:`,
                    event.target.error
                );
            };
        };
        if (!isPending && openRequest.result) {
            onSuccess();
        } else {
            openRequest.addEventListener('success', onSuccess, { once: true });
        }
    });
}
