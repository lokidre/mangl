import React, { useRef, useEffect, useContext } from 'react';
import { styled } from '@mui/material';
import { Height } from '@mui/icons-material';
import { GlobalStateInstance } from 'GlobalStateProvider';
// import {GlobalStateInstance} from '../GlobalStateProvider';
// import {GlobalContext} from 'GlobalContext';


//import wasm_module_js from '../wasm/wasm_module.js';

let mouseIsDown = false;




// const handleRightClick = (event) => {
//     const localX = event.clientX - event.target.offsetLeft;
//     const localY = event.clientY - event.target.offsetTop;

//     // setlocalRightClickPos({ x: localX, y: localY });
//     console.log('Mouse right click', localX, localY);
// };

const CanvasElement = styled('canvas')({});
export const wasmCanvasId = 'wasm_canvas';



export default function View() {
    const canvasRef = React.useRef();
    const stateCtx = React.useContext(GlobalStateInstance).state;

    // console.log('Height', global.canvasHeight);

    // React.useEffect(() => {

    //     let wasmScript = document.createElement("script");
    //     wasmScript.src = 'static://wasm_module.js';
    //     //wasmScript.src = wasm_module_js;
    //     document.body.appendChild(wasmScript);

    //     wasmScript = document.createElement("script");
    //     wasmScript.src = `static://mangl_workshop_wasm.js`;
    //     document.body.appendChild(wasmScript);


    // }, []);



    React.useEffect(() => {
        if (!canvasRef.current) return;


        let foundScript = document.getElementById("draw_canvas_js_id")
        if (foundScript)
            return;

        let moduleScript = document.createElement("script");
        moduleScript.id = "draw_canvas_js_id";
        moduleScript.src = '/draw_canvas.js';
        //document.body.appendChild(moduleScript);

        let wasmScript = document.createElement("script");
        wasmScript.id = "wasm_render_js_id";
        wasmScript.src = '/mangl_workshop_wasm.js';
        //document.body.appendChild(wasmScript);

        document.body.append(moduleScript, wasmScript);


    }, canvasRef.current)


    const handleMouseDown = React.useCallback((event) => {
        let localX = event.clientX - event.target.offsetLeft;
        let localY = event.clientY - event.target.offsetTop;


        localY = stateCtx.canvasHeight - 1 - localY;


        mouseIsDown = true
        //window['wasmOnMouseDown'](localX, localY); ////Module.
        Module.onMouseDown(localX, localY);

    }, []);

    const handleMouseUp = React.useCallback((event) => {
        let localX = event.clientX - event.target.offsetLeft;
        let localY = event.clientY - event.target.offsetTop;

        localY = stateCtx.canvasHeight - 1 - localY;

        mouseIsDown = false
        Module.onMouseUp(localX, localY); ////Module.


    }, []);

    const handleMouseMove = React.useCallback((event) => {

        if (mouseIsDown) {
            let localX = event.clientX - event.target.offsetLeft;
            let localY = event.clientY - event.target.offsetTop;


            localY = stateCtx.canvasHeight - 1 - localY;

            //window['wasmOnMouseMove'](localX, localY); ////Module.
            Module.onMouseMove(localX, localY);
        }



    }, []);

    const handleMouseLeave = React.useCallback((event) => {

        if (mouseIsDown) {
            let localX = event.clientX - event.target.offsetLeft;
            let localY = event.clientY - event.target.offsetTop;


            localY = stateCtx.canvasHeight - 1 - localY;


            mouseIsDown = false

            Module.onMouseLeave(localX, localY); ////Module.
        }



    }, []);


    return (
        <CanvasElement
            id={wasmCanvasId}
            ref={canvasRef}
            onMouseDown={handleMouseDown}
            onMouseUp={handleMouseUp}
            onMouseMove={handleMouseMove}
            onMouseLeave={handleMouseLeave}
            sx={{
                width: stateCtx.canvasWidth,
                height: stateCtx.canvasHeight,
                borderStyle: 'solid',
                borderWidth: 2,
                borderColor: 'black',


                backgroundColor: '#eee',
                backgroundImage: `linear-gradient(45deg, #4786c9 25%, transparent 25%, transparent 75%, #4786c9 75%, #4786c9), 
        linear-gradient(45deg, #4786c9 25%, transparent 25%, transparent 75%, #4786c9 75%, #4786c9)`,
                backgroundSize: '64px 64px',
                backgroundPosition: '0 0, 32px 32px',
            }}
        />
    );
}
