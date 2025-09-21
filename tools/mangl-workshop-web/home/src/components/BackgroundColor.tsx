import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Autocomplete, Button, Checkbox, FormControlLabel, FormGroup, TextField, Typography } from '@mui/material';
import { useContext, useEffect, useRef, useState } from 'react';
import ColorPickerComponent from './ColorPickerComponent';






export default function BackgroundColor() {

    const [backgroundColor, setBackgroundColor] = React.useState("#ffff");

    const setColoredBackground = (color: string) => {
        window['backgroundColorSelect'](color); ////Module.
    };



    return (
        <Box sx={{ display: 'flex', flexDirection: 'row' }}>
            <ColorPickerComponent label='background color' value={backgroundColor} onValueChange={setBackgroundColor} />
            <Button variant='contained' size="small" sx={{ display: 'inline', m: 1 }} onClick={() => setColoredBackground(backgroundColor)}>Apply</Button>
        </Box>
    );
}