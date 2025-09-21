import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Autocomplete, Button, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import { GlobalStateInstance } from 'GlobalStateProvider';
import AllowUsageCheckbox from './AllowUsageCheckbox';
import View, { wasmCanvasId } from './screen';






export default function Sizing() {

    const stateCtx = useContext(GlobalStateInstance).state;


    const deviceSizes: any[] = [
        { label: 'Phone', size: [320, 480] },
        { label: 'Square', size: [300, 300] },
        { label: 'Desktop', size: [480, 640] },
        { label: 'Banner', size: [320, 50] },
        { label: 'Pad', size: [384, 512] },

    ]

    const [selectedOptions, setSelectedOptions] = useState({ size: [stateCtx.canvasWidth, stateCtx.canvasHeight] });
    const [selectedCustomOptions, setelectedCustomOptions] = useState({ width: 0, height: 0 });
    const [customSizingIsDisabled, setCustomSizingIsDisabled] = useState(true);


    const isMounted = useRef(false);


    const handleChange = (event, value) => setSelectedOptions(value || { size: [stateCtx.canvasWidth, stateCtx.canvasHeight] });

    const handleCustomChange = (event) => {
        if (event.target.id === 'width') {
            selectedCustomOptions.width = event.target.value
        } else {
            selectedCustomOptions.height = event.target.value
        }
    }



    function manualSizeChange() {

        const view = document.getElementById(wasmCanvasId)
        view.style.width = `${selectedCustomOptions.width}px`
        view.style.height = `${selectedCustomOptions.height}px`
        console.log('Custom', selectedCustomOptions)
        stateCtx.canvasWidth = selectedCustomOptions.width
        stateCtx.canvasHeight = selectedCustomOptions.height


    }



    useEffect(() => {
        if (isMounted.current) {
            const view = document.getElementById(wasmCanvasId)
            view.style.width = `${selectedOptions.size[0]}px`
            view.style.height = `${selectedOptions.size[1]}px`
            stateCtx.canvasWidth = selectedOptions.size[0]
            stateCtx.canvasHeight = selectedOptions.size[1]
        } else {
            isMounted.current = true;

        }
    }, [selectedOptions]);


    return (
        <Box>
            <Typography>Default Sizes</Typography>
            <Autocomplete
                disablePortal
                options={deviceSizes}
                // defaultValue={deviceSizes[0]}
                onChange={handleChange}
                isOptionEqualToValue={(option, value) => option.label === value.label}
                sx={{ width: '50%', mt: 2}}
                renderInput={(params) => <TextField {...params} label="Devices" />}

            />
            <Typography sx={{ mt: 5 }}>Custom Sizes</Typography>

            <AllowUsageCheckbox label="Custom Sizing" allowed={customSizingIsDisabled} onAllowUsage={setCustomSizingIsDisabled} onClick={() => {}} />

            <TextField
                id="width"
                label="Width"
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

                InputProps={{
                    inputProps: {
                        min: 0
                    },
                }}

                onChange={handleCustomChange}
                required
                disabled={customSizingIsDisabled}

            />
            <TextField
                id="height"
                label="Height"
                type="number"
                onChange={handleCustomChange}
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
                InputProps={{
                    inputProps: {
                        min: 0
                    },
                }}
                required
                disabled={customSizingIsDisabled}
            />
            <Button onClick={manualSizeChange} disabled={customSizingIsDisabled} variant="contained" size="small" sx={{ display: 'inline', m: 1 }}>Update</Button>
        </Box>
    );
}