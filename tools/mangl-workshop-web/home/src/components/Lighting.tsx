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






export default function Lighting() {
    const stateCtx = React.useContext(GlobalStateInstance).state;

    const [ambientColor, setAmbientColor] = useState(stateCtx.ambientColor);
    const [diffuseColor, setDiffuseColor] = useState(stateCtx.diffuseColor);
    const [specularColor, setSpecularColor] = useState(stateCtx.specularColor);

    const [lighting, setLighting] = useState(stateCtx.lighing);
    const [shininess, setShininess] = React.useState(stateCtx.shininess)


    const [lightX, setLightX] = useState(stateCtx.lightX)
    const [lightY, setLightY] = useState(stateCtx.lightY)
    const [lightZ, setLightZ] = useState(stateCtx.lightZ)


    const enableLighting = () => {

        setLighting(!lighting)
        let localLighting = !lighting
        stateCtx.lighing = localLighting
        localStorage.setItem('lighting', localLighting.toString())
        Module.call("setModelLighting", localLighting.toString());

    }

    const setLightPosition = () => {
        let lighting = [lightX, lightY, lightZ]
        Module.call("setModelLightingPosition", JSON.stringify(lighting))
        localStorage.setItem('lightX', lightX)
        localStorage.setItem('lightY', lightY)
        localStorage.setItem('lightZ', lightZ)
        stateCtx.lightX = lightX
        stateCtx.lightY = lightY
        stateCtx.lightZ = lightZ
    }


    const changeMaterial = () => {
        let material = [ambientColor, diffuseColor, specularColor, shininess]
        Module.call("setModelMaterial", JSON.stringify(material))
        localStorage.setItem('shininess', shininess)
        localStorage.setItem('ambientColor', ambientColor)
        localStorage.setItem('diffuseColor', diffuseColor)
        localStorage.setItem('specularColor', specularColor)
        stateCtx.ambientColor = ambientColor
        stateCtx.diffuseColor = diffuseColor
        stateCtx.specularColor = specularColor
        stateCtx.shininess = shininess

    }





    return (
        <Box>
            <AllowUsageCheckbox label="Enable" initial={lighting} allowed={lighting} onAllowUsage={setLighting} onClick={enableLighting} />
            <TextField
                id="outlined-number"
                label="X"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    m: 1,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                onChange={(x) => setLightX(x.target.value)}
                defaultValue={lightX}
                disabled={!lighting}
            />
            <TextField
                id="outlined-number"
                label="Y"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    m: 1,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                onChange={(y) => setLightY(y.target.value)}
                defaultValue={lightY}
                disabled={!lighting}
            />
            <TextField
                id="outlined-number"
                label="Z"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    m: 1,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                onChange={(z) => setLightZ(z.target.value)}
                defaultValue={lightZ}
                disabled={!lighting}
            />

            <Button onClick={setLightPosition} size='small' sx={{ display: 'inline', mt: 1, height: 30}}><CheckCircleIcon sx={{ m: 0, p: 0 }} ></CheckCircleIcon></Button>



            <Box sx={{ display: 'flex', flexDirection: 'row', mt: 3}}>
                <ColorPickerComponent label='Ambient' value={ambientColor} onValueChange={setAmbientColor} />
                <ColorPickerComponent label='Diffuse' value={diffuseColor} onValueChange={setDiffuseColor} />
                <ColorPickerComponent label='Specular' value={specularColor} onValueChange={setSpecularColor} />

                <TextField
                id=""
                label="shininess"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    m: 1,
                    width: 80,
                    mt: 4,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                defaultValue={shininess}
                onChange={(shine) => setShininess(shine.target.value)}
            />

            <Button onClick={changeMaterial} size='small' sx={{ display: 'inline', mt: 4, height: 30}}><CheckCircleIcon sx={{ m: 0, p: 0 }} ></CheckCircleIcon></Button>
            </Box>

            


        </Box>
    );
}
