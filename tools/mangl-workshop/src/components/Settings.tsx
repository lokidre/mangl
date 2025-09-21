import * as React from 'react';
import Box from '@mui/material/Box';


import { styled as muiStyled } from '@mui/system';
import { Accordion, AccordionDetails, AccordionSummary, Autocomplete, Button, Checkbox, Divider, Drawer, FormControlLabel, FormGroup, Stack, TextField, Typography } from '@mui/material';
import TimeSlider from './TimeSlider';
import { BlockPicker, SketchPicker, HuePicker } from "react-color";

import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
import { useEffect, useRef, useState } from 'react';
import { theme_ } from 'theme';
import View, { wasmCanvasId } from './screen';
import ColorPickerComponent from './ColorPickerComponent';
import { GlobalStateInstance } from '../GlobalStateProvider';

import AllowUsageCheckbox from './AllowUsageCheckbox';



const ImageComponent = muiStyled('img')({});

const SettingBoxWidth = '25%'



export default function Settings() {

    const global = React.useContext(GlobalStateInstance).state



    const deviceSizes: any[] = [
        { label: 'Phone', size: ['320px', '480px'] },
        { label: 'Square', size: ['300px', '300px'] },
        { label: 'Desktop', size: ['480px', '640px'] },
        { label: 'Banner', size: ['320px', '50px'] },
        { label: 'Pad', size: ['384px', '512px'] },

    ]


    // const [ambientColorVisibility, setAmbientColorVisibility] = useState('none');
    // const [diffuseColorVisibility, setDiffuseColorVisibility] = useState('none');
    // const [specularColorVisibility, setSpecularColorVisibility] = useState('none');

    const [ambientColor, setAmbientColor] = useState("#37d67a");
    const [diffuseColor, setDiffuseColor] = useState("#37d67a");
    const [specularColor, setSpecularColor] = useState("#37d67a");

    const [lightingIsDisabled, setLightingIsDisabled] = useState(true);
    const [customSizingIsDisabled, setCustomSizingIsDisabled] = useState(true);

    const [selectedOptions, setSelectedOptions] = useState(deviceSizes[0]);
    const [selectedCustomOptions, setelectedCustomOptions] = useState({ width: 0, height: 0 });

    const isMounted = useRef(false);


    const handleChange = (event, value) => setSelectedOptions(value || deviceSizes[0]);

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
        global.canvasWidth = selectedCustomOptions.width
        global.canvasHeight = selectedCustomOptions.height
        console.log('Height', global.canvasHeight)

    }



    useEffect(() => {
        if (isMounted.current) {
            const view = document.getElementById(wasmCanvasId)
            view.style.width = selectedOptions.size[0]
            view.style.height = selectedOptions.size[1]
            console.log('Regular', selectedOptions)
            global.canvasWidth = selectedOptions.size[0]
            global.canvasHeight = selectedOptions.size[1]

        } else {
            isMounted.current = true;
        }
    }, [selectedOptions]);




    return (
        <Box sx={{ height: '100%' }}>
            <Drawer
                sx={{
                    width: SettingBoxWidth,
                    flexShrink: 0,
                    '& .MuiDrawer-paper': {
                        width: SettingBoxWidth,
                        boxSizing: 'border-box',
                    },
                }}
                variant="permanent"
                anchor="right"
            >
                <div>
                    <Accordion>
                        <AccordionSummary
                            expandIcon={<ExpandMoreIcon />}
                            aria-controls="panel1a-content"
                            id="panel1a-header"
                        >
                            <Typography>Size</Typography>
                        </AccordionSummary>
                        <AccordionDetails>
                            <Typography>Default Sizes</Typography>
                            <Autocomplete
                                disablePortal
                                id="combo-box-demo"
                                options={deviceSizes}
                                defaultValue={deviceSizes[0]}
                                onChange={handleChange}
                                isOptionEqualToValue={(option, value) => option.label === value.label}
                                sx={{ width: '50%', m: 2 }}
                                renderInput={(params) => <TextField {...params} label="Devices" />}

                            />
                            <Typography sx={{ mt: 5 }}>Custom Sizes</Typography>

                            <AllowUsageCheckbox label="Custom Sizing" allowed={customSizingIsDisabled} onAllowUsage={setCustomSizingIsDisabled} />

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
                        </AccordionDetails>
                    </Accordion>
                    <Accordion>
                        <AccordionSummary
                            expandIcon={<ExpandMoreIcon />}
                            aria-controls="panel2a-content"
                            id="panel2a-header"
                        >
                            <Typography>Model</Typography>
                        </AccordionSummary>
                        <AccordionDetails>
                            <AllowUsageCheckbox label="Lighting" allowed={lightingIsDisabled} onAllowUsage={setLightingIsDisabled} />
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
                                disabled={lightingIsDisabled}
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
                                disabled={lightingIsDisabled}
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
                                disabled={lightingIsDisabled}
                            />


                            <Box sx={{ p: 2, display: 'flex', flexDirection: 'row' }}>
                                <ColorPickerComponent label='Ambient' value={ambientColor} onValueChange={setAmbientColor} />
                                <ColorPickerComponent label='Diffuse' value={diffuseColor} onValueChange={setDiffuseColor} />
                                <ColorPickerComponent label='Specular' value={specularColor} onValueChange={setSpecularColor} />
                            </Box>



                            <TextField
                                id=""
                                label="shininess"
                                type="number"
                                InputLabelProps={{
                                    shrink: true,
                                }}
                                sx={{
                                    m: 1,
                                    width: 75,
                                    display: 'block',
                                    mt: 2,
                                    "& .MuiInputBase-root": {
                                        height: 30
                                    }
                                }}
                            />

                            <TextField
                                id=""
                                label="scale"
                                type="number"
                                InputLabelProps={{
                                    shrink: true,
                                }}
                                sx={{
                                    m: 1,
                                    width: 75,
                                    display: 'block',
                                    "& .MuiInputBase-root": {
                                        height: 30
                                    }
                                }}
                                InputProps={{
                                    inputProps: {
                                        step: 0.1,
                                        max: 2,
                                        min: 0
                                    },
                                }}
                            />


                            <FormGroup sx={{ display: 'flex', flexDirection: 'row', mt: 2 }}>
                                <FormControlLabel control={<Checkbox />} label="Wireframe" />
                                <FormControlLabel control={<Checkbox />} label="Vertices" />
                                <FormControlLabel control={<Checkbox />} label="Face Culling" />
                                <FormControlLabel control={<Checkbox />} label="Depth Test" />
                                <FormControlLabel control={<Checkbox />} label="Lables" />
                            </FormGroup>

                        </AccordionDetails>
                    </Accordion>
                    <Accordion>
                        <AccordionSummary
                            expandIcon={<ExpandMoreIcon />}
                            aria-controls="panel3a-content"
                            id="panel3a-header"
                        >
                            <Typography>Debug</Typography>
                        </AccordionSummary>
                        <AccordionDetails>
                            <FormGroup>
                                <FormControlLabel control={<Checkbox />} label="Wireframe" />
                                <FormControlLabel control={<Checkbox />} label="Vertices" />
                                <FormControlLabel control={<Checkbox />} label="Axes" />
                                <FormControlLabel control={<Checkbox />} label="Bounds" />
                                <FormControlLabel control={<Checkbox />} label="Lables" />
                            </FormGroup>
                        </AccordionDetails>
                    </Accordion>
                </div>
            </Drawer >
        </Box >
    );


}
