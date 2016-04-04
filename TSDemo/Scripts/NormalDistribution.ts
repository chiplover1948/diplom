export function Normal(n: number): Array<Array<number>> {
    var res = new Array<Array<number>>();
    for (var i = 0; i < n; i++) {
        res.push(NormalPair());
    }
    return res;
}

function NormalPair(): Array<number> {
    var x: number, y: number, s: number;
    do {
        x = Math.random() * 2 - 1;
        y = Math.random() * 2 - 1;
        s = Math.pow(x, 2) + Math.pow(y, 2);
    } while (s > 1 || s === 0);
    var t =  Math.sqrt((-2) * Math.log(s) / s);
    return [x * t * 0.2, y * t * 0.2];    
}