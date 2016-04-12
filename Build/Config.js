require.config({
    paths: {
        'solver': '../lib/solver/js-solver',
        'idd': '../lib/IDD/idd',
        'jquery': '../lib/jquery/jquery',
        'modernizer': "../lib/modernizr/modernizr",
        "rx": "../lib/rxjs/rx.all",
        "knockout": "../lib/knockout/knockout",
        "tinycolor2": "../lib/tinycolor/tinycolor"
    },
    shim: {
        'solver': {
            exports: 'Module'
        }
    }
});
