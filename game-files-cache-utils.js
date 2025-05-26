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
    const putRequest = store.put(blob, filePath);
    await new Promise((resolve, reject) => {
        putRequest.onsuccess = () => {
            console.log(`${filePath} stored to indexedDB successfully!`);
            resolve();
        };
        putRequest.onerror = (event) => {
            reject(`Error storing ${filePath} to indexedDB: ${event.target.error}`);
        };
    })
}

/**
 * @param {IDBOpenDBRequest} openRequest
 * @param {string} filePath
 * @returns {Promise<undefined | ArrayBuffer>}
 */
export function getFileFromIndexedDb(openRequest, filePath) {
    return new Promise(resolve => {
        const onError = () => {
            resolve(undefined);
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

            getRequest.onsuccess = (event) => {
                /** @type {Blob | undefined} */
                const blob = event.target.result;

                if (!blob) {
                    resolve(undefined);
                    return;
                }

                blob.arrayBuffer().then(resolve)
            };

            getRequest.onerror = (event) => {
                resolve(undefined);
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
