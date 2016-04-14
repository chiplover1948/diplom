define(["require", "exports"], function (require, exports) {
    function convertOpts(opts) {
        var Opts = new Module.Options();
        Opts.OutputStep = opts.OutputStep;
        return Opts;
    }
    function convertFunc(Right, n) {
        return Module.Runtime.addFunction(function (t, x, r) {
            var right = new Array(n);
            for (var i = 0; i < n; i++) {
                right[i] = Module.getValue(x + i * 8, 'double');
            }
            var dx = Right(t, right);
            Module.HEAPF64.set(new Float64Array(dx), r / 8);
        });
    }
    function convertVector(vec) {
        var res = new Module.Vector(vec.length);
        vec.forEach(function (val, i) { res.SetElement(i, val); });
        return res;
    }
    function convertVectorBack(vec) {
        var res = new Array();
        for (var i = 0; i < 12; i++)
            res[i] = vec.GetElement(i);
        return res;
    }
    var GearSolver = (function () {
        function GearSolver(t0, x0, f, opts) {
            this.pointer = convertFunc(f, x0.length);
            this.solver = new Module.Gear(t0, convertVector(x0), this.pointer, convertOpts(opts));
            if (opts.OutputStep > 0) {
                this.xout = new Array();
                this.tout = t0 + opts.OutputStep;
            }
        }
        GearSolver.prototype.Solve = function () {
            if (this.OutputStep > 0) {
                do {
                    var s = this.solver.Solve();
                    var time = s.Time();
                    this.xout = convertVectorBack(s.Solve());
                    s.delete();
                } while (time < this.tout);
            }
            var s = this.solver.Solve();
            var res = { time: s.Time(), solve: convertVectorBack(s.Solve()) };
            s.delete();
            return res;
        };
        GearSolver.prototype.dispose = function () {
            this.solver.delete();
            Module.Runtime.removeFunction(this.pointer);
        };
        return GearSolver;
    })();
    exports.GearSolver = GearSolver;
});
