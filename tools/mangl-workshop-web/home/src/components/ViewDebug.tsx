import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Autocomplete, Button, Checkbox, FormControlLabel, FormGroup, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import { GlobalStateInstance } from 'GlobalStateProvider';
import AllowUsageCheckbox from './AllowUsageCheckbox';
import View, { wasmCanvasId } from './screen';






export default function ViewDebug() {



    return (
        <Box>
            <FormGroup sx={{ display: 'flex', flexDirection: 'row' }}>
                <FormControlLabel control={<Checkbox />} label="Wireframe" />
                <FormControlLabel control={<Checkbox />} label="Vertices" />
                <FormControlLabel control={<Checkbox />} label="Axes" />
                <FormControlLabel control={<Checkbox />} label="Bounds" />
                <FormControlLabel control={<Checkbox />} label="Lables" />
            </FormGroup>
        </Box>
    );
}