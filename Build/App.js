define(["require", "exports", "jquery", "knockout", "idd", "tinycolor2", "Build/PrecompiledScripts/idd-ko.js"], function (require, exports, $, ko, idd, tinycolor) {
    function initVector_() {
        var p = {};
        p.deg = 0.0008;
        p.cat = 0.0008;
        p.pol = 0.0167;
        p.nick = 0.0167;
        p.x0 = 4;
        p.ann = 0.01;
        p.bind = 5.4E-06;
        p.bind2 = 0.001;
        p.bind1 = 5E-05;
        p.unbind = 0.1126;
        p.Cmax = 1000;
        p.c = 0.0008;
        p.kt = 0.001;
        p.ku = 0.001;
        p.s = 2;
        p.e = 2.71828183;
        p.R = 0.0019872;
        p.T = 298.15;
        var x0 = new Array(106);
        for (var i = 0; i < 106; i++)
            x0[i] = 0;
        x0[0] = p.Cmax;
        x0[1] = p.Cmax;
        x0[2] = p.Cmax;
        x0[3] = p.Cmax;
        x0[4] = p.Cmax;
        x0[5] = p.Cmax;
        x0[6] = p.Cmax;
        x0[7] = p.Cmax;
        x0[8] = p.Cmax;
        x0[9] = p.Cmax;
        x0[10] = p.Cmax;
        x0[11] = p.Cmax;
        x0[12] = p.Cmax;
        x0[13] = p.Cmax;
        x0[14] = p.Cmax;
        x0[15] = p.Cmax;
        x0[16] = p.Cmax;
        x0[17] = p.Cmax;
        x0[18] = p.Cmax;
        x0[19] = p.Cmax;
        x0[20] = p.Cmax;
        x0[21] = p.Cmax;
        x0[22] = p.Cmax;
        x0[23] = p.Cmax;
        x0[24] = p.Cmax;
        x0[25] = p.Cmax;
        x0[26] = p.Cmax;
        x0[27] = p.Cmax;
        x0[28] = p.Cmax;
        x0[29] = p.Cmax;
        x0[30] = p.Cmax;
        x0[31] = p.Cmax;
        x0[32] = p.Cmax;
        x0[33] = p.Cmax;
        x0[34] = p.Cmax;
        x0[35] = p.Cmax;
        x0[36] = p.Cmax;
        x0[37] = p.Cmax;
        x0[38] = p.Cmax;
        x0[39] = p.Cmax;
        x0[42] = p.x0;
        return x0;
    }
    function initVector() {
        var x0 = new Array(12);
        for (var i = 0; i < 12; i++)
            x0[i] = 0;
        x0[2] = 4;
        return x0;
    }
    var Solve = (function () {
        function Solve() {
            var _this = this;
            this.t = new Float64Array(0);
            this.x = ko.observableArray([]);
            this.color = ko.observable("");
            this.upper = new Array();
            this.lower = new Array();
            this.Lower = ko.pureComputed(function () {
                if (_this.lower.length == 0) {
                    for (var i = 0; i < _this.t.length; i++) {
                        _this.lower.push(_this.x()[0][i]);
                        _this.upper.push(_this.x()[0][i]);
                    }
                }
                else {
                    for (var i = 0; i < _this.t.length; i++) {
                        _this.lower[i] = Math.min(_this.lower[i], _this.x()[_this.x().length - 1][i]);
                        _this.upper[i] = Math.max(_this.upper[i], _this.x()[_this.x().length - 1][i]);
                    }
                }
                return _this.lower;
            });
            this.middle = ko.pureComputed(function () {
                var arr = new Array();
                if (_this.t.length == 0)
                    return arr;
                for (var i = 0; i < _this.t.length; i++) {
                    var tmp = new Array();
                    for (var j = 0; j < _this.x().length; j++) {
                        tmp.push(_this.x()[j][i]);
                    }
                    tmp.sort();
                    arr.push(tmp[Math.floor(tmp.length / 2)]);
                }
                return arr;
            });
            this.name = "species";
            this.CIColor = ko.computed(function () {
                var c = tinycolor(_this.color());
                c.setAlpha(0.2);
                return c.toRgbString();
            });
        }
        return Solve;
    })();
    var ViewModel = (function () {
        function ViewModel() {
            this.solves = ko.observableArray([]);
            this.sigma = ko.observable(0.2);
            this.count = ko.observable(50);
            this.step = ko.observable(200);
            this.averagePoint = ko.observable(0);
            this.averageSolution = ko.observable(0);
            this.t = ko.observableArray([]);
            ko.applyBindings(this);
        }
        ViewModel.prototype.compute = function () {
            var _this = this;
            if (this.worker != undefined)
                this.worker.terminate();
            this.worker = new Worker("./Build/Workers/bootWorker.js");
            this.solves.removeAll();
            this.averagePoint(0);
            this.averageSolution(0);
            this.averagesPoint = new Array();
            this.averagesSolution = new Array();
            var solve1 = new Solve();
            var solve2 = new Solve();
            var solve3 = new Solve();
            var solve4 = new Solve();
            solve1.color('green');
            solve1.name = "L - L'";
            solve2.color('red');
            solve2.name = "R - R'";
            solve3.color('blue');
            solve3.name = "V - V'";
            solve4.color('grey');
            solve4.name = "Y - Y'";
            this.worker.onmessage = function (ev) {
                var data = ev.data;
                var Time = new Float64Array(data.Time);
                var Solves = new Array(106);
                for (var i = 0; i < 106; i++)
                    Solves[i] = new Float64Array(data.Solves[i]);
                solve1.x.push(Solves[40].map(function (val, i) { return val - Solves[41][i]; }));
                solve2.x.push(Solves[42].map(function (val, i) { return val - Solves[64][i]; }));
                solve3.x.push(Solves[57].map(function (val, i) { return val - Solves[84][i]; }));
                solve4.x.push(Solves[98].map(function (val, i) { return val - Solves[102][i]; }));
                if (solve1.t.length == 0) {
                    solve1.t = solve2.t = solve3.t = solve4.t = Time;
                    _this.solves.push(solve1, solve2, solve3, solve4);
                }
                _this.averagesPoint.push(data.AverageTime);
                _this.averagesSolution.push(data.TotalTime);
                _this.averagePoint(_this.averagesPoint.reduce(function (a, b) { return a + b; }) / _this.averagesPoint.length);
                _this.averageSolution(_this.averagesSolution.reduce(function (a, b) { return a + b; }) / _this.averagesSolution.length);
            };
            var message = {
                x0: initVector_(),
                t0: 0,
                options: { OutputStep: typeof this.step() == 'string' ? parseFloat(this.step()) : this.step() },
                rightSide: "rightSide106.js",
                sigma: typeof this.sigma() == 'string' ? parseFloat(this.sigma()) : this.sigma(),
                count: typeof this.count() == 'string' ? parseFloat(this.count()) : this.count()
            };
            this.worker.postMessage(JSON.stringify(message));
        };
        return ViewModel;
    })();
    $(document).ready(function () {
        new ViewModel();
        idd.asPlot($("div[data-idd-plot='chart']"));
    });
});
