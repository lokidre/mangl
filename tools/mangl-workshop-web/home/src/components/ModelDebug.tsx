import * as React from 'react';
import Box from '@mui/material/Box';
import { Autocomplete, Button, Checkbox, FormControlLabel, FormGroup, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import { GlobalStateInstance } from 'GlobalStateProvider';
import AllowUsageCheckbox from './AllowUsageCheckbox';
import View, { wasmCanvasId } from './screen';
import parseBool from 'parse';




// function getBoolSetting(name: string, defaultOption: boolean) {
//     let item = localStorage.getItem(name)
//     if (item) {
//         return parseBool(item)
//     } else {
//         return defaultOption
//     }

// }

export default function ModelDebug() {

    // const [settings, setSettings] = React.useState<any | null>(null);
    // let storedSettings = JSON.parse(localStorage.getItem('settings'))
    const stateCtx = React.useContext(GlobalStateInstance).state;


    const [wireFrame, setWireFrame] = React.useState(stateCtx.wireFrame)
    const [vertices, setVertices] = React.useState(stateCtx.vertices)
    const [faceCulling, setFaceCulling] = React.useState(stateCtx.faceCulling)
    const [depthTest, setDepthTest] = React.useState(stateCtx.depthTest)



    const enableWireframe = () => {

        setWireFrame(!wireFrame)
        let localWireFrame = !wireFrame
        localStorage.setItem('wireFrame', localWireFrame.toString())
        stateCtx.wireFrame = localWireFrame
        Module.call("setModelWireframe", localWireFrame.toString());
    }

    const enableVertices = () => {
        setVertices(!vertices)
        let localVertices = !vertices
        localStorage.setItem('vertices', localVertices.toString())
        stateCtx.vertices = localVertices
        Module.call("setModelVertices", localVertices.toString());
    }

    const enableFaceCulling = () => {
        

        setFaceCulling(!faceCulling)
        let localFaceCulling = !faceCulling

        localStorage.setItem('faceCulling', localFaceCulling.toString())
        stateCtx.faceCulling = localFaceCulling
        Module.call("setModelFaceCulling", localFaceCulling.toString());
    }

    const enableDepthTest = () => {

        setDepthTest(!depthTest)
        let localDepthTest = !depthTest
        localStorage.setItem('depthTest', localDepthTest.toString())
        stateCtx.depthTest = localDepthTest
        Module.call("setModelDepthTest", localDepthTest.toString());
    }




    return (
        <Box>
            <FormGroup sx={{ display: 'flex', flexDirection: 'row' }}>
                <FormControlLabel control={<Checkbox checked={wireFrame} onChange={enableWireframe} />} label="Wireframe" />
                <FormControlLabel control={<Checkbox checked={vertices} onClick={enableVertices} />} label="Vertices" />
                <FormControlLabel control={<Checkbox checked={faceCulling} onClick={enableFaceCulling} />} label="Face Culling" />
                <FormControlLabel control={<Checkbox checked={depthTest} onClick={enableDepthTest} />} label="Depth Test" />
                <FormControlLabel control={<Checkbox />} label="Lables" />
            </FormGroup>
        </Box>
    );
}