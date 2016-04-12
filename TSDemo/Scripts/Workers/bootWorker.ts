/// <reference path="../../typings/requirejs/require.d.ts" />

importScripts("../../lib/requirejs/require.js");
importScripts("../../lib/solver/js-solver.js");

var message;

require(["./mainWorker"], ( MainWorker) => {
    self.onmessage = MainWorker;
    if (message !== undefined)
        self.onmessage(message)
});

self.onmessage = (ev: MessageEvent) => {
    message = ev
}
