/// <reference path="../typings/requirejs/require.d.ts" />
/// <reference path="../typings/emscripten/emscripten.d.ts" />
/// <reference path="../typings/jquery/jquery.d.ts" />
/// <reference path="./solver.d.ts" />
/// <reference path="./idd.d.ts" />

import $ = require("jquery");
import Module = require("solver");
import InteractiveDataDisplay = require("idd");

function setArray() {
    var n = 500;
    var t = new Array(n);
    var x0 = new Array(n);
    var x1 = new Array(n);
    var x2 = new Array(n);
    
    var vec = new Module.Vector(3);
    vec.SetElement(0, 1.0);
    vec.SetElement(1, 0.0);
    vec.SetElement(2, 0.0);
    
    var opts = new Module.Options();
    t[0] = 0.0; x0[0] = 1.0, x1[0] = 0.0; x2[0] = 0.0;
    var solver = Module.ChemistrySolver(0.0, vec, opts);
    
    
    var timeStart = Date.now();
    for (var i = 1; i < n; i++) {
        var s = solver.Solve();
        t[i] = s.Time();
        x0[i] = s.Solve().GetElement(0);
        x1[i] = s.Solve().GetElement(1) * 1e3; 
        x2[i] = s.Solve().GetElement(2);
    }			
    var timeEnd = Date.now();
    console.log(timeEnd - timeStart);
    InteractiveDataDisplay.asPlot('chart').polyline("p0", { x: t, y: x0, stroke: 'Green', thickness: 2 });
    InteractiveDataDisplay.asPlot('chart').polyline("p1", { x: t, y: x1, stroke: 'Red', thickness: 2 });
    InteractiveDataDisplay.asPlot('chart').polyline("p2", { x: t, y: x2, stroke: 'Blue', thickness: 2 });
}


$(document).ready(function() {			
    var chart = InteractiveDataDisplay.asPlot('chart');
    setArray();
});