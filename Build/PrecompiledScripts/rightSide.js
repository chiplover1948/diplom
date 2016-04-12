
function rightSide (t, x) {
    var p = {};
    p.deg = 0.0008;
    p.cat = 0.0008;
    p.pol = 0.0167;
    p.nick = 0.0167;
    p.x0 = 4;
    p.ann = 0.01;
    p.bind = 5.4e-06;
    p.bind2 = 0.001;
    p.bind1 = 5e-05;
    p.unbind = 0.1126;
    p.Cmax = 1000;
    p.c = 0.0008;
    p.kt = 0.001;
    p.ku = 0.001;
    p.s = 2;
    p.e = 2.71828183;
    p.R = 0.0019872;
    p.T = 298.15;
    var deg = p.deg;
    var cat = p.cat;
    var pol = p.pol;
    var nick = p.nick;
    var x0 = p.x0;
    var ann = p.ann;
    var bind = p.bind;
    var bind2 = p.bind2;
    var bind1 = p.bind1;
    var unbind = p.unbind;
    var Cmax = p.Cmax;
    var c = p.c;
    var kt = p.kt;
    var ku = p.ku;
    var s = p.s;
    var e = p.e;
    var T = p.T;

    // Assign states
    var L = x[0];
    var L$ = x[1];
    var R = x[2];
    var R$ = x[3];
    var V = x[4];
    var V$ = x[5];
    var Y = x[6];
    var Y$ = x[7];
    var sp9 = x[8];
    var sp10 = x[9];
    var sp11 = x[10];
    var sp12 = x[11];

    // Define reaction propensities
    var r_0 = (cat * R);
    var r_1 = (cat * R$);
    var r_2 = (cat * Y);
    var r_3 = (cat * Y$);
    var r_4 = (deg * sp9);
    var r_5 = (deg * sp10);
    var r_6 = (cat * sp9);
    var r_7 = (cat * sp10);
    var r_8 = (cat * sp9);
    var r_9 = (cat * sp10);
    var r_10 = (cat * sp11);
    var r_11 = (cat * sp12);
    var r_12 = (deg * V);
    var r_13 = (deg * V$);
    var r_14 = ((ann * sp12) * sp11);
    var r_15 = (0.2 * V);
    var r_16 = (0.2 * V$);
    var r_17 = (0.1 * Y);
    var r_18 = (0.1 * Y$);
    var r_19 = ((0.1 * Y) * Y$);
    var r_20 = ((0.01 * Y) * L);
    var r_21 = ((0.01 * Y$) * L$);
    var r_22 = (L$ * L);
    var r_23 = ((ann * R$) * R);
    var r_24 = ((ann * V$) * V);
    var r_25 = ((ann * sp10) * sp9);

    // Assign derivatives
    var dL = -r_22;
    var dL$ = -r_22;
    var dR = -r_23;
    var dR$ = -r_23;
    var dV = r_8 + r_10 - r_12 - r_24;
    var dV$ = r_9 + r_11 - r_13 - r_24;
    var dY = r_15 - r_17 - r_19 - r_20;
    var dY$ = r_16 - r_18 - r_19 - r_21;
    var dsp9 = r_0 + r_3 - r_4 - r_25;
    var dsp10 = r_1 + r_2 - r_5 - r_25;
    var dsp11 = r_6 - r_14;
    var dsp12 = r_7 - r_14;

    return [dL, dL$, dR, dR$, dV, dV$, dY, dY$, dsp9, dsp10, dsp11, dsp12]; 
}