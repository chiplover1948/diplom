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
        for (var i = 0; i < vec.Length(); i++)
            res[i] = vec.GetElement(i);
        return res;
    }
    var GearSolver = (function () {
        function GearSolver(t0, x0, f, opts) {
            this.pointer = convertFunc(f, x0.length);
            this.opts = convertOpts(opts);
            this.x0 = convertVector(x0);
            this.solver = new Module.Gear(t0, this.x0, this.pointer, this.opts);
        }
        GearSolver.prototype.Solve = function () {
            var s = this.solver.Solve();
            var res = { time: s.Time(), solve: convertVectorBack(s.Solve()) };
            s.delete();
            return res;
        };
        GearSolver.prototype.dispose = function () {
            this.solver.delete();
            this.opts.delete();
            this.x0.delete();
            Module.Runtime.removeFunction(this.pointer);
        };
        return GearSolver;
    })();
    exports.GearSolver = GearSolver;
});
