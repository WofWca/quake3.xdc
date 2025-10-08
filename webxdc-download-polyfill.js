(() => {
  const originalAnchorClick = HTMLAnchorElement.prototype.click;
  HTMLAnchorElement.prototype.click = function () {
    if (
      !this.hasAttribute("download") ||
      !this.href ||
      document.contains(this)
    ) {
      return originalAnchorClick.call(this);
    }

    const urlObj = new URL(this.href);
    if (
      !(
        this.href.startsWith("blob:") ||
        this.href.startsWith("data:") ||
        urlObj.origin === location.origin
      )
    ) {
      return originalAnchorClick.call(this);
    }

    exportFileFromUrl(this.href, this.download);
  };

  document.addEventListener("click", (event) => {
    const link = event.target.closest("a[download]");
    if (!link || !link.href) return;
    if (link.href.startsWith("blob:") || link.href.startsWith("data:")) {
      event.preventDefault();
    } else {
      const urlObj = new URL(link.href);
      if (urlObj.origin === location.origin) {
        event.preventDefault();
      } else return;
    }

    exportFileFromUrl(link.href, link.download);
  });

  async function exportFileFromUrl(url, name) {
    const message = {
      file: {},
    };
    if (url.startsWith("blob:")) {
      const blob = await (await fetch(url)).blob();
      message.file.blob = blob;
    } else if (url.startsWith("data:")) {
      const [meta, encoded] = url.split(",");
      const isBase64 = meta.endsWith(";base64");
      if (isBase64) {
        message.file.base64 = encoded;
      } else {
        message.file.plainText = decodeURIComponent(encoded);
      }
    } else {
      const blob = await (await fetch(url)).blob();
      message.file.blob = blob;
      if (!name) {
        const urlObj = new URL(url);
        name = urlObj.pathname.split("?")[0].split("/").pop();
      }
    }
    message.file.name = name || "file";
    window.webxdc.sendToChat(message);
  }
})();
