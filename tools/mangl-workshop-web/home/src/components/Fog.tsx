import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Autocomplete, Button, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import { GlobalStateInstance } from 'GlobalStateProvider';
import AllowUsageCheckbox from './AllowUsageCheckbox';
import View, { wasmCanvasId } from './screen';
import ColorPickerComponent from './ColorPickerComponent';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';


import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import FormControl from '@mui/material/FormControl';
import Select, { SelectChangeEvent } from '@mui/material/Select';



export default function Fog() {

    const stateCtx = React.useContext(GlobalStateInstance).state;


    // const [lightingIsDisabled, setLightingIsDisabled] = useState(true);


    const [type, setType] = useState(stateCtx.fogType);
    const [depthType, setDepthType] = useState(stateCtx.fogDepth);
    const [density, setDensity] = useState(stateCtx.fogDensity);
    const [zNear, setZnear] = useState(stateCtx.zNear);
    const [zFar, setZfar] = useState(stateCtx.zFar);
    const [fogColor, setFogColor] = useState(stateCtx.fogColor);


    const handleTypeChange = (event: SelectChangeEvent) => {
        setType(event.target.value);
    };

    const handleDepthChange = (event: SelectChangeEvent) => {
        setDepthType(event.target.value);
    };



    const setFog = () => {
        const fog = {type: type, depthType: depthType, color: fogColor, density: density, zNear: zNear, zFar: zFar}
        localStorage.setItem('fogType', type)
        localStorage.setItem('fogDepth', depthType)
        localStorage.setItem('fogColor', fogColor)
        localStorage.setItem('fogDensity', density)
        localStorage.setItem('zNear', zNear)
        localStorage.setItem('zFar', zFar)
        stateCtx.fogType = type
        stateCtx.fogDepth = depthType
        stateCtx.fogColor = fogColor
        stateCtx.fogDensity = density
        stateCtx.zNear = zNear
        stateCtx.zFar = zFar
        Module.call('setModelFog', JSON.stringify(fog))
    }

    const types = [
        'None',
        'Linear',
        'Exponential',
        'ExpSquare',
    ];

    const depth = [
        'distance',
        'Range'
    ];

    return (
        <Box>
            <FormControl sx={{ width: '30%' }}>
                <InputLabel id="demo-simple-select-label">Type</InputLabel>
                <Select
                    labelId="demo-simple-select-label"
                    id="demo-simple-select"
                    value={type}
                    label="type"
                    onChange={handleTypeChange}
                >
                    {types.map((type) => (
                        <MenuItem
                            key={type}
                            value={type}
                        >
                            {type}
                        </MenuItem>
                    ))}
                </Select>
            </FormControl>

            <FormControl sx={{ width: '30%', ml: 2 }}>
                <InputLabel id="demo-simple-select-label">Depth</InputLabel>
                <Select
                    labelId="demo-simple-select-label"
                    id="demo-simple-select"
                    value={depthType}
                    label="Depth"
                    onChange={handleDepthChange}
                >
                    {depth.map((type) => (
                        <MenuItem
                            key={type}
                            value={type}
                        >
                            {type}
                        </MenuItem>
                    ))}
                </Select>
            </FormControl>


            <Box sx={{ mt: 2 }}>
                <ColorPickerComponent label='Fog color' value={fogColor} onValueChange={setFogColor} />
            </Box>


            <TextField

                label="Density"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    mt: 3,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                defaultValue={density}
                onChange={(density) => setDensity(density.target.value)}
            />

            <TextField

                label="Znear"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    mt: 3,
                    ml: 1,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                defaultValue={zNear}
                onChange={(zNear) => setZnear(zNear.target.value)}
            />

            <TextField

                label="Zfar"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    mt: 3,
                    ml: 1,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                defaultValue = {zFar}
                onChange={(zFar) => setZfar(zFar.target.value)}
            />

            <Button onClick={setFog} variant='contained' size='small' sx={{ display: 'block', mt: 2, height: 30}}>Apply</Button>
        </Box>

    );



    // return (
    //     <Box>
    //         <AllowUsageCheckbox label="Enable" allowed={lightingIsDisabled} onAllowUsage={setLightingIsDisabled} />
    //         <TextField
    //             id="outlined-number"
    //             label="X"
    //             type="number"
    //             InputLabelProps={{
    //                 shrink: true,
    //             }}
    //             sx={{
    //                 m: 1,
    //                 width: 75,
    //                 "& .MuiInputBase-root": {
    //                     height: 30
    //                 }
    //             }}
    //             disabled={lightingIsDisabled}
    //         />
    //         <TextField
    //             id="outlined-number"
    //             label="Y"
    //             type="number"
    //             InputLabelProps={{
    //                 shrink: true,
    //             }}
    //             sx={{
    //                 m: 1,
    //                 width: 75,
    //                 "& .MuiInputBase-root": {
    //                     height: 30
    //                 }
    //             }}
    //             disabled={lightingIsDisabled}
    //         />
    //         <TextField
    //             id="outlined-number"
    //             label="Z"
    //             type="number"
    //             InputLabelProps={{
    //                 shrink: true,
    //             }}
    //             sx={{
    //                 m: 1,
    //                 width: 75,
    //                 "& .MuiInputBase-root": {
    //                     height: 30
    //                 }
    //             }}
    //             disabled={lightingIsDisabled}
    //         />



    //         <Box sx={{ display: 'flex', flexDirection: 'row' }}>
    //             <ColorPickerComponent label='Ambient' value={ambientColor} onValueChange={setAmbientColor} />
    //             <ColorPickerComponent label='Diffuse' value={diffuseColor} onValueChange={setDiffuseColor} />
    //             <ColorPickerComponent label='Specular' value={specularColor} onValueChange={setSpecularColor} />
    //         </Box>
    //     </Box>
    // );
}

