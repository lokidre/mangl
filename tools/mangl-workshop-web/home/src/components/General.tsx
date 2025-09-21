import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Autocomplete, Button, Checkbox, FormControlLabel, FormGroup, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import { GlobalStateInstance } from 'GlobalStateProvider';
import AllowUsageCheckbox from './AllowUsageCheckbox';
import View, { wasmCanvasId } from './screen';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';





export default function Debug() {

    const stateCtx = React.useContext(GlobalStateInstance).state;

    const [scale, setScale] = React.useState(stateCtx.scale)


    const changeScale = () => {
        Module.call("setModelScale", scale)
        stateCtx.scale = scale
        localStorage.setItem('scale', scale)
    }



    return (
        <Box sx={{ display: 'flex', flexDirection: 'row' }}>
           

            <TextField
                id=""
                label="scale"
                type="number"
                InputLabelProps={{
                    shrink: true,
                }}
                sx={{
                    m: 2,
                    width: 75,
                    "& .MuiInputBase-root": {
                        height: 30
                    }
                }}
                InputProps={{
                    inputProps: {
                        step: 0.1,
                        max: 10,
                        min: 0
                    },
                }}
                defaultValue={scale}
                onChange={(scale) => setScale(scale.target.value)}
            />

            <Button onClick={changeScale} size='small' sx={{ display: 'inline', mt: 2, height: 30, width: 10 }}><CheckCircleIcon sx={{m: 0, p: 0}} ></CheckCircleIcon></Button>

        </Box>
    );
}