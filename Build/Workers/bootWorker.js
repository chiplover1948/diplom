importScripts("../../lib/requirejs/require.js");
importScripts("../../lib/solver/js-solver.js");
var message;
require(["./mainWorker"], function (MainWorker) {
    self.onmessage = MainWorker;
    if (message !== undefined)
        self.onmessage(message);
});
self.onmessage = function (ev) {
    message = ev;
};
