export function Normal(n: number, m: number, s: number): Array<number> {
    var res = new Array<number>();
    var i = 0;
    while (i < n) {
        var rand = NormalPair().map(val => m + s * val);
        
        i = res.push(rand[0], rand[1]);
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
    return [x * t, y * t];    
}