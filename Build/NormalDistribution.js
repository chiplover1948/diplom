define(["require", "exports"], function (require, exports) {
    function Normal(n, m, s) {
        var res = new Array();
        var i = 0;
        while (i < n) {
            var rand = NormalPair().map(function (val) { return m + s * val; });
            i = res.push(rand[0], rand[1]);
        }
        return res;
    }
    exports.Normal = Normal;
    function NormalPair() {
        var x, y, s;
        do {
            x = Math.random() * 2 - 1;
            y = Math.random() * 2 - 1;
            s = Math.pow(x, 2) + Math.pow(y, 2);
        } while (s > 1 || s === 0);
        var t = Math.sqrt((-2) * Math.log(s) / s);
        return [x * t, y * t];
    }
});
