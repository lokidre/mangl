import React, {useRef, useEffect, useContext} from 'react';
import {styled} from '@mui/material';
import {GlobalStateInstance} from '../GlobalStateProvider';
import {GlobalContext} from 'GlobalContext';

//import wasm_module_js from '../wasm/wasm_module.js';

const handleMouseClick = (event) => {
    const localX = event.clientX - event.target.offsetLeft;
    const localY = event.clientY - event.target.offsetTop;

    // setLocalMousePos({ x: localX, y: localY });
    console.log('Mouse click', localX, localY);
};

const handleRightClick = (event) => {
    const localX = event.clientX - event.target.offsetLeft;
    const localY = event.clientY - event.target.offsetTop;

    // setlocalRightClickPos({ x: localX, y: localY });
    console.log('Mouse right click', localX, localY);
};

const CanvasElement = styled('canvas')({});
export const wasmCanvasId = 'wasm_canvas';

export default function View() {
    const global = React.useContext(GlobalStateInstance).state;
    console.log('Height', global.canvasHeight);

    React.useEffect(() => {

        let wasmScript = document.createElement("script");
        wasmScript.src = 'static://wasm_module.js';
        //wasmScript.src = wasm_module_js;
        document.body.appendChild(wasmScript);

        wasmScript = document.createElement("script");
        wasmScript.src = `static://mangl_workshop_wasm.js`;
        document.body.appendChild(wasmScript);


    }, []);

    return (
        <CanvasElement
            id={wasmCanvasId}
            onClick={handleMouseClick}
            onContextMenu={handleRightClick}
            sx={{
                width: `${global.canvasWidth}px`,
                height: `${global.canvasHeight}px`,
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
