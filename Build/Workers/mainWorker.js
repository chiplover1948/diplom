define(["require", "exports", '../Solver', '../NormalDistribution'], function (require, exports, Solver, NormalDistribution_1) {
    return function (ev) {
        var message = JSON.parse(ev.data);
        var initials = NormalDistribution_1.Normal(message.count, 4, message.sigma);
        var initials1 = NormalDistribution_1.Normal(message.count, 8, message.sigma);
        var initials2 = NormalDistribution_1.Normal(message.count, 8, message.sigma);
        var currInitial = 0;
        var Result;
        importScripts("../PrecompiledScripts/" + message.rightSide);
        initials.forEach(function (val, count) {
            var time = new Array();
            var solves = new Array(12);
            for (var i = 0; i < 12; i++) {
                solves[i] = new Array();
            }
            message.x0[2] = val;
            var gear = new Solver.GearSolver(message.t0, message.x0, rightSide, message.options);
            var s = { solve: message.x0, time: message.t0 };
            do {
                time.push(s.time);
                solves.forEach(function (v, i) {
                    v.push(s.solve[i]);
                });
                s = gear.Solve();
            } while (s.time <= 50000);
            gear.dispose();
            var lastSolve = solves.map(function (val) { return val[val.length - 1]; });
            lastSolve[3] = initials1[count];
            var s = { solve: lastSolve, time: 50000 };
            var gear = new Solver.GearSolver(50000, lastSolve, rightSide, message.options);
            while (s.time <= 100000) {
                time.push(s.time);
                solves.forEach(function (v, i) {
                    v.push(s.solve[i]);
                });
                s = gear.Solve();
            }
            gear.dispose();
            var lastSolve = solves.map(function (val) { return val[val.length - 1]; });
            lastSolve[2] = initials2[count];
            var s = { solve: lastSolve, time: 100000 };
            var gear = new Solver.GearSolver(100000, lastSolve, rightSide, message.options);
            while (s.time < 150000) {
                time.push(s.time);
                solves.forEach(function (v, i) {
                    v.push(s.solve[i]);
                });
                s = gear.Solve();
            }
            gear.dispose();
            var resultTime = new Float64Array(time.length);
            for (var k = 0; k < time.length; k++)
                resultTime[k] = time[k];
            var resultSolves = new Array(12);
            for (var k = 0; k < 12; k++) {
                resultSolves[k] = new Float64Array(time.length);
                for (var h = 0; h < time.length; h++)
                    resultSolves[k][h] = solves[k][h];
            }
            var returnTransfer = [resultTime.buffer];
            resultSolves.forEach(function (val) { return returnTransfer.push(val.buffer); });
            self.postMessage({
                Time: resultTime.buffer,
                Solves: resultSolves.map(function (val) { return val.buffer; })
            }, returnTransfer);
        });
    };
});
