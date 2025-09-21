
var onWasmError = (err) => {
    alert(err);
};



var Module = {
    noInitialRun: true, // do not call main()

    canvas: (function () { return document.getElementById('wasm_canvas'); })(),

    onRuntimeInitialized: () => {

        var canvas = document.getElementById('wasm_canvas');
        canvas.width = 320;
        canvas.height = 480;




        Module.initialize();
        Module.start();
    },
};

/*
    emscripten::function("onMouseDown", &mangl::onWasmMouseDown);
    emscripten::function("onMouseUp", &mangl::onWasmMouseUp);
    emscripten::function("onMouseMove", &mangl::onWasmMouseMove);
    emscripten::function("onMouseLeave", &mangl::onWasmMouseLeave);
*/

// function wasmOnMouseDown(x, y) {
//     Module.onMouseDown(x, y);
// }

// function wasmOnMouseUp(x, y) {
//     Module.onMouseUp(x, y);
// }

// function wasmOnMouseMove(x, y) {
//     Module.onMouseMove(x, y);
// }

// function wasmOnMouseLeave(x, y) {
//     Module.onMouseLeave(x, y);
// }

function fileSelect(file_name, tab, file) {
    console.log('file name', file_name)
    // console.log('file tab', tab)
    // console.log('file', file)
}


function backgroundColorSelect(color) {
    console.log('color', color)
    
}


function startAnimation() {
    console.log('starting animation')
    return 100
    
}

function stopAnimation() {
    console.log('stopping animation')
    
}


// function setWireframe(wireframe) {
//     Module.call('setModelWireframe', wireframe)
// }
