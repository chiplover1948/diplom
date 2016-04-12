import * as Solver from '../Solver';
import {Normal} from '../NormalDistribution'

declare var rightSide;

export = (ev: MessageEvent) => {
    var message = <Solver.IWorkerMessage>JSON.parse(ev.data);
    var initials = Normal(message.count, 4, message.sigma);
    
    var currInitial = 0;
    
    var Result: Solver.IWorkerResult;
    
    
    
    importScripts("../PrecompiledScripts/" + message.rightSide);
    initials.forEach((val) => {
        var time = new Array<number>();
        var solves = new Array<Array<number>>(12);
        for (var i = 0; i < 12; i++) {
            solves[i] = new Array<number>();        
        }
        message.x0[2] = val;
        var gear = new Solver.GearSolver(message.t0, message.x0, rightSide, message.options);
        var s = {solve: message.x0, time: message.t0};
        while (s.time < message.tFinal) {
            time.push(s.time);
            solves.forEach((v, i) => {
                v.push(s.solve[i]);
            });
            s = gear.Solve();
        }
        gear.dispose();
        
        
        var resultTime = new Float64Array(time.length);
        for (var k = 0; k < time.length; k++)
            resultTime[k] = time[k];
        var resultSolves = new Array<Float64Array>(12);
        for (var k = 0; k < 12; k++) {
            resultSolves[k] = new Float64Array(time.length);
            for (var h = 0; h < time.length; h++)
                resultSolves[k][h] = solves[k][h];
        }
        var returnTransfer = [resultTime.buffer];
        resultSolves.forEach(val => returnTransfer.push(val.buffer));
        (<any>self).postMessage(<Solver.IWorkerResult>{
            Time: resultTime.buffer, 
            Solves: resultSolves.map(val => val.buffer)
        }, returnTransfer);
        
        
    });    
}