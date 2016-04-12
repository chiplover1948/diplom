define(["require", "exports"], function (require, exports) {
    function convertOpts(opts) {
        var Opts = new Module.Options();
        Opts.OutputStep = opts.OutputStep;
        Opts.AbsoluteTolerance = 1e-8;
        Opts.RelativeTolerance = 1e-6;
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
        }
        GearSolver.prototype.Solve = function () {
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
