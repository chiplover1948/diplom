/// <reference path="../typings/requirejs/require.d.ts" />

require.config({
    baseUrl: './Build',
    paths: {
        'solver': '../lib/solver/js-solver',
        'idd': '../lib/IDD/idd',
        'jquery': '../lib/jquery/jquery',
        'modernizer': "https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min",        
        "rx": "https://cdnjs.cloudflare.com/ajax/libs/rxjs/3.1.2/rx.lite.min",
    },
    shim: {
        'solver': {
            exports: 'Module'
        }
    }
});