/// <reference path="./solver.d.ts" />
/// <reference path="../typings/rx/rx.d.ts" />

export interface IOptions {
    
}

export interface IVector {
    vector: Array<number>;
    size: number;
}

export interface ISolution {
    time: number;
    solve: IVector;
}

export interface IWorkerMessage {
    options: IOptions;
    rightSide: string;
    x0: IVector;
    t0: number;
    tFinal: number;
}

export interface IRightSide {
    (t: number, v: Array<number>): Array<number>;
}

function convertOpts(opts: IOptions): Module.Options {
    return new Module.Options();
}

function convertFunc(Right: IRightSide, n: number) {
    return Module.Runtime.addFunction((t, x, r) => {
                var right = new Array<number>(n);
                for (var i = 0; i < n; i++) {
                    right[i] = Module.getValue(x + i * 8, 'double');
                }
                var dx = Right(t, right);
                Module.HEAPF64.set(new Float64Array(dx), r/8);
            });
}

function convertVector(vec: IVector): Module.Vector {
    var res = new Module.Vector(vec.size);
    vec.vector.forEach((val, i) => {res.SetElement(i, val)});
    return res;
}

function convertVectorBack(vec: Module.Vector): IVector {
    var res: IVector = {vector: new Array<number>(), size: 12};
    for(var i = 0; i < 12; i++)
        res.vector[i] = vec.GetElement(i);
    return res;
}


export class GearSolver {
    solver: Module.Gear;
    
    constructor(t0: number, x0: IVector, f: IRightSide, opts: IOptions) {
        this.solver = new Module.Gear(t0, convertVector(x0), convertFunc(f, x0.size), convertOpts(opts));      
    }
    
    Solve(): ISolution {
        var s = this.solver.Solve();
        var res = {time: s.Time(), solve: convertVectorBack(s.Solve())};
        s.delete();
        return res;       
    }
    
    dispose() {
        this.solver.delete();
    }
}