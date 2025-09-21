var onWasmError = (err) => {
    alert(err);
};

const wasmCanvasId = 'wasm_canvas';


var Module = {
    noInitialRun: true, // do not call main()

    canvas: (function() { return document.getElementById(wasmCanvasId); })(),

    onRuntimeInitialized: () => {

        var canvas = document.getElementById(wasmCanvasId);
        canvas.width = 414;
        canvas.height = 896;


        //var mainApp = new Module.ManglApplication();
        //mainApp.initialize();
        // Module.initialize();
        // Module.start();
    },
};

