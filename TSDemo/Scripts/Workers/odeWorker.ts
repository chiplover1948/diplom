/// <reference path="../PrecompiledScripts/solver.d.ts" />
/// <reference path="../../typings/requirejs/require.d.ts" />

importScripts("../../lib/solver/js-solver.js");
importScripts("../../lib/requirejs/require.js");

var message;
declare var rightSide;
/** The temporary callback. All it does is store the message. */
var storeMessage = function (e) { message = e; }

require(['../Solver'], (Solver) => {
    self.onmessage = function (ev) {
        var msg = JSON.parse(ev.data);
        importScripts("../PrecompiledScripts/" + msg.rightSide);
        var gear = new Solver.GearSolver(msg.t0, msg.x0, rightSide, msg.options);
        var s = {solve: msg.x0, time: msg.t0};
        while (s.time < msg.tFinal) {
        (<any>self).postMessage(s);
            s = gear.Solve();
        }
        gear.dispose();
        (<any>self).postMessage("end");
    }

    if (message != undefined)
        self.onmessage(message);
    
})

self.onmessage = storeMessage;
