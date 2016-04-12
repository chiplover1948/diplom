importScripts("../../lib/solver/js-solver.js");
importScripts("../../lib/requirejs/require.js");
var message;
var storeMessage = function (e) { message = e; };
require(['../Solver'], function (Solver) {
    self.onmessage = function (ev) {
        var msg = JSON.parse(ev.data);
        importScripts("../PrecompiledScripts/" + msg.rightSide);
        var gear = new Solver.GearSolver(msg.t0, msg.x0, rightSide, msg.options);
        var s = { solve: msg.x0, time: msg.t0 };
        while (s.time < msg.tFinal) {
            self.postMessage(s);
            s = gear.Solve();
        }
        gear.dispose();
        self.postMessage("end");
    };
    if (message != undefined)
        self.onmessage(message);
});
self.onmessage = storeMessage;
