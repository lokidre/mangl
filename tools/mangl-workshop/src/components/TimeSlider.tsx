import * as React from 'react';
import Box from '@mui/material/Box';
import Slider from '@mui/material/Slider';
import { Button } from '@mui/material';

const marks = [
  {
    value: 0,
    label: '0s',
  },
  {
    value: 10,
    label: '10s',
  },
  {
    value: 20,
    label: '20s',
  },
  {
    value: 30,
    label: '30s',
  },
  {
    value: 40,
    label: '40s',
  },
  {
    value: 50,
    label: '50s',
  },
  {
    value: 60,
    label: '60s',
  },
  {
    value: 70,
    label: '70s',
  },
  {
    value: 80,
    label: '80s',
  },
  {
    value: 90,
    label: '90s',
  },
  {
    value: 100,
    label: '100s',
  }
];

function valuetext(value: number) {
  return `${value}°C`;
}

export default function TimeSlider() {
  return (
    <Box sx={{ width: '50%' }}>
      <Slider
        aria-label="Custom marks"
        defaultValue={0}
        getAriaValueText={valuetext}
        step={10}
        valueLabelDisplay="auto"
        marks={marks}
      />
      <Box>
        <Button sx={{margin: 1}} variant='contained'>Play</Button>
        <Button sx={{margin: 1}} variant='contained'>Stop</Button>
        <Button sx={{float: 'right', margin: 1}} variant='contained'>Close</Button>
      </Box>
    </Box>
  );
}