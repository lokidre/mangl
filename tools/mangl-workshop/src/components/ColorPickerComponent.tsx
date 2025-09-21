import * as React from 'react';
import { Container, Box, Typography, Popover, Button } from "@mui/material";
import { BlockPicker, SketchPicker, HuePicker } from "react-color";
import ClickAwayListener from '@mui/base/ClickAwayListener';

interface colorPickerProps {
    label: string,
    value: string,
    onValueChange: (color: string) => void;
    //onClose: () => void
}


export default function ColorPickerComponent(props: colorPickerProps) {
    const [pickerVisible, setPickerVisible] = React.useState(false);

    const [colorPickerVisibility, setColorPickerVisibility] = React.useState('none')
    const [sketchPickerColor, setSketchPickerColor] = React.useState(props.value);

    const [anchorEl, setAnchorEl] = React.useState<HTMLButtonElement | null>(null);

    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
        setAnchorEl(event.currentTarget);
    };

    const handleClose = () => {
        setAnchorEl(null);
    };

    const open = Boolean(anchorEl);
    const id = open ? 'colorPicker-popover' : undefined;


    // const activateColorPicker = () => {
    //     if (colorPickerVisibility === 'none') {
    //         setColorPickerVisibility('block')
    //     } else {
    //         setColorPickerVisibility('none')
    //     }
    // }

    return (

        <Box sx={{display: 'inline'}}>
            <Box sx={{
                display: 'flex',
                alignItems: 'center',
                flexWrap: 'wrap',
            }}>
                <Typography>{props.label}: </Typography>
                <Button sx={{ "&:hover": { backgroundColor: "transparent", } }} aria-describedby={id} onClick={handleClick}>
                    <Box sx={{ m: 1, width: 20, height: 20, backgroundColor: props.value }}></Box>
                </Button>
            </Box>


            <Popover
                id={id}
                open={open}
                anchorEl={anchorEl}
                onClose={handleClose}
                anchorOrigin={{
                    vertical: 'bottom',
                    horizontal: 'left',
                }}
            > <SketchPicker
                    color={props.value}
                    onChange={(color) => props.onValueChange(color.hex)}
                />
            </Popover>

            {/* <Box sx={{ display: `${colorPickerVisibility}` }}>
                    <SketchPicker
                        color={props.value}
                        onChange={props.onValueChange}
                    />
                </Box> */}
        </Box>

    );


}



