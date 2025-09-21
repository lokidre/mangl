import * as React from 'react';
import Box from '@mui/material/Box';


import { styled as muiStyled } from '@mui/system';
import { Stack, Typography } from '@mui/material';
import TimeSlider from './TimeSlider';
import { useEffect, useState } from 'react';
import View from './screen';


const ImageComponent = muiStyled('img')({});

const viewBoxWidth = '100%';

interface Props {
    /**
     * Injected by the documentation to work in an iframe.
     * You won't need it on your project.
     */
    window?: () => Window;
}

export default function ViewBox(props: Props) {

    // const [globalMousePos, setGlobalMousePos] = useState({});
    // const [localMousePos, setLocalMousePos] = useState({});
    // const [localRightClickPos, setlocalRightClickPos] = useState({})

    const handleMouseClick = (event) => {
        const localX = event.clientX - event.target.offsetLeft;
        const localY = event.clientY - event.target.offsetTop;

        // setLocalMousePos({ x: localX, y: localY });
        console.log('Mouse click', localX, localY)
    };

    const handleRightClick = (event) => {
        const localX = event.clientX - event.target.offsetLeft;
        const localY = event.clientY - event.target.offsetTop;

        // setlocalRightClickPos({ x: localX, y: localY });
        console.log('Mouse right click', localX, localY)

    };

    // const handleDrag = (event) => {
    //     console.log(event.clientX, event.clientY)
    // }

    // useEffect(() => {
    //     const handleMouseMove = (event) => {
    //         setGlobalMousePos({
    //             x: event.clientX,
    //             y: event.clientY,
    //         });
    //     };

    //     window.addEventListener('mousemove', handleMouseMove);

    //     return () => {
    //         window.removeEventListener(
    //             'mousemove',
    //             handleMouseMove
    //         );
    //     };
    // }, []);




    return (
        <Stack direction="column" alignItems='center' justifyContent='center' spacing={4} sx={{ height: '100%'}}> 
        {/* optional border for view (borderRight: '5px solid #535454', borderLeft: '5px solid #535454') */}
            <Box sx={{ mt: '10%', width: '100%', textAlign: 'center' }}>
                <View />
                {/* <ImageComponent onClick={handleMouseClick} onContextMenu={handleRightClick} sx={{ width: '100%', height: '600px' }} src="images/editView.png" /> */}
                {/* <Box draggable onDrag={handleDrag} sx={{width: '50px', height: '50px', backgroundColor: 'red'}}></Box> */}
            </Box>
            <TimeSlider />
        </Stack>
    );
}
