import * as React from 'react';
import Box from '@mui/material/Box';


import { styled as muiStyled } from '@mui/system';
import { Accordion, AccordionDetails, AccordionSummary, Autocomplete, Button, Checkbox, Divider, Drawer, FormControlLabel, FormGroup, Stack, Tab, Tabs, TextField, Typography } from '@mui/material';
import TimeSlider from './TimeSlider';
import { BlockPicker, SketchPicker, HuePicker } from "react-color";

import ExpandMoreIcon from '@mui/icons-material/ExpandMore';
import { useEffect, useRef, useState } from 'react';
import { theme_ } from 'theme';
import View, { wasmCanvasId } from './screen';
import ColorPickerComponent from './ColorPickerComponent';
// import { GlobalStateInstance } from '../GlobalStateProvider';

import AllowUsageCheckbox from './AllowUsageCheckbox';
import { GlobalStateInstance } from 'GlobalStateProvider';
import Sizing from './Sizing';
import General from './General';
import Lighting from './Lighting';
import ModelDebug from './ModelDebug';
import ViewDebug from './ViewDebug';
import Fog from './Fog';
import BackgroundColor from './BackgroundColor';
import parseBool from 'parse';



const ImageComponent = muiStyled('img')({});

const SettingBoxWidth = '25%'

interface TabPanelProps {
    children?: React.ReactNode;
    index: number;
    value: number;
}

function TabPanel(props: TabPanelProps) {
    const { children, value, index, ...other } = props;

    return (
        <Box
            role="tabpanel"
            hidden={value !== index}
            id={`simple-tabpanel-${index}`}
            aria-labelledby={`simple-tab-${index}`}
            {...other}
        >
            {value === index && (
                <Box sx={{ p: 0 }}>
                    <Typography component={'span'}>{children}</Typography>
                </Box>
            )}
        </Box>
    );
}

function a11yProps(index: number) {
    return {
        id: `simple-tab-${index}`,
        'aria-controls': `simple-tabpanel-${index}`,
    };
}


export default function Settings() {

    const stateCtx = React.useContext(GlobalStateInstance).state;

    const [value, setValue] = React.useState(0);

    const handleTabChange = (event: React.SyntheticEvent, newValue: number) => {
        setValue(newValue);
    };

    

    React.useEffect(() => {

        const timer = setTimeout(() => {
          
            // Module.call('setModel', stateCtx.model[0].file)
            // Module.call("setModelWireframe", stateCtx.wireFrame.toString());
            // Module.call("setModelVertices", stateCtx.vertices.toString());
            // Module.call("setModelFaceCulling", stateCtx.faceCulling.toString());
            // Module.call("setModelDepthTest", stateCtx.depthTest.toString());
            // Module.call("setModelLighting", stateCtx.lighing.toString());
            // Module.call("setModelScale", stateCtx.scale);
            // Module.call("setModelLightingPosition", JSON.stringify([stateCtx.lightX, stateCtx.lightY, stateCtx.lightZ]));
            // Module.call("setModelMaterial", JSON.stringify([stateCtx.ambientColor, stateCtx.diffuseColor, stateCtx.specularColor, stateCtx.shininess]));
            // Module.call("setModelFog", JSON.stringify([stateCtx.fogType, stateCtx.fogDepth, stateCtx.fogColor, stateCtx.fogDensity, stateCtx.zNear, stateCtx.zFar]));

            if( Object.keys(stateCtx.currentFile).length > 0) {
                Module.call(stateCtx.currentFile.callFunction, stateCtx.currentFile.file)
                console.log('I am here')
            }
            
            stateCtx.loadModelProperties();

            Module.modelData_ ='asd'


        }, 1000);
        return () => clearTimeout(timer);

    }, [])



    // const [ambientColorVisibility, setAmbientColorVisibility] = useState('none');
    // const [diffuseColorVisibility, setDiffuseColorVisibility] = useState('none');
    // const [specularColorVisibility, setSpecularColorVisibility] = useState('none');

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
                <Box sx={{ width: '100%' }}>
                    <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
                        <Tabs value={value} onChange={handleTabChange} textColor='inherit' aria-label="basic tabs example">
                            <Tab label="View" {...a11yProps(0)} />
                            <Tab label="Model" {...a11yProps(1)} />
                        </Tabs>
                    </Box>
                    <TabPanel value={value} index={0}>

                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel1a-content"
                                id="panel1a-header"
                            >
                                <Typography>Size</Typography>
                            </AccordionSummary>
                            <AccordionDetails>
                                <Sizing />
                            </AccordionDetails>
                        </Accordion>

                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel1a-content"
                                id="panel1a-header"
                            >
                                <Typography>Background</Typography>
                            </AccordionSummary>
                            <AccordionDetails>
                                <BackgroundColor />
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
                                <ViewDebug />
                            </AccordionDetails>
                        </Accordion>

                    </TabPanel>

                    <TabPanel value={value} index={1}>

                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel2a-content"
                                id="panel2a-header"
                            >
                                <Typography>General</Typography>
                            </AccordionSummary>
                            <AccordionDetails>
                                <General />
                            </AccordionDetails>
                        </Accordion>


                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel2a-content"
                                id="panel2a-header"
                            >
                                <Typography>Lighting</Typography>
                            </AccordionSummary>
                            <AccordionDetails>
                                <Lighting />
                            </AccordionDetails>
                        </Accordion>


                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel2a-content"
                                id="panel2a-header"
                            >
                                <Typography>Debug</Typography>

                            </AccordionSummary>
                            <AccordionDetails>
                                <ModelDebug />
                            </AccordionDetails>
                        </Accordion>

                        <Accordion>
                            <AccordionSummary
                                expandIcon={<ExpandMoreIcon />}
                                aria-controls="panel2a-content"
                                id="panel2a-header"
                            >
                                <Typography>Fog</Typography>
                            </AccordionSummary>
                            <AccordionDetails>
                                <Fog />
                            </AccordionDetails>
                        </Accordion>
                    </TabPanel>
                </Box>
            </Drawer >
        </Box >
    );


}
