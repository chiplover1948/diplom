define(["require", "exports", "jquery", "knockout", "idd", "tinycolor2", "Build/PrecompiledScripts/idd-ko.js"], function (require, exports, $, ko, idd, tinycolor) {
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
            this.t = ko.observableArray([]);
            ko.applyBindings(this);
        }
        ViewModel.prototype.compute = function () {
            var _this = this;
            if (this.worker != undefined)
                this.worker.terminate();
            this.worker = new Worker("./Build/Workers/bootWorker.js");
            this.solves.removeAll();
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
                var Solves = new Array(12);
                for (var i = 0; i < 12; i++)
                    Solves[i] = new Float64Array(data.Solves[i]);
                solve1.x.push(Solves[0].map(function (val, i) { return val - Solves[1][i]; }));
                solve2.x.push(Solves[2].map(function (val, i) { return val - Solves[3][i]; }));
                solve3.x.push(Solves[4].map(function (val, i) { return val - Solves[5][i]; }));
                solve4.x.push(Solves[6].map(function (val, i) { return val - Solves[7][i]; }));
                if (solve1.t.length == 0) {
                    solve1.t = solve2.t = solve3.t = solve4.t = Time;
                    _this.solves.push(solve1, solve2, solve3, solve4);
                }
            };
            var message = {
                x0: initVector(),
                t0: 0,
                options: { OutputStep: typeof this.step() == 'string' ? parseFloat(this.step()) : this.step() },
                rightSide: "rightSide.js",
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
