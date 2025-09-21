import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Button } from '@mui/material';



function valuetext(value: number) {
  return `${value}°C`;
}

export default function TimeSlider() {

  const startAnimation = () => {
    window['startAnimation']()
  }

  const stopAnimation = () => {
    window['stopAnimation']()
  }



  return (
    <Box sx={{ width: '50%' }}>
      <Slider
        aria-label="Custom marks"
        defaultValue={0}
        getAriaValueText={valuetext}
        step={1}
        valueLabelDisplay="auto"
        marks
        min={1}
        max={10}
      />
      <Box sx={{textAlign: 'center'}}>
        <Button onClick={startAnimation} sx={{ margin: 1 }} variant='contained'>Start</Button>
        <Button onClick={stopAnimation} sx={{ margin: 1 }} variant='contained'>Stop</Button>
        {/* <Button sx={{ float: 'right', margin: 1 }} variant='contained'>Close</Button> */}
      </Box>
    </Box>
  );
}