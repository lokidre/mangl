import * as React from 'react';
import { Container, Box, Typography, Popover, Button, FormGroup, FormControlLabel, Checkbox } from "@mui/material";


interface allowUsageProps {
    label: string,
    allowed: boolean,
    initial?: boolean
    onAllowUsage: (allowed: boolean) => void;
    onClick?: () => void;

}


export default function AllowUsageCheckbox(props: allowUsageProps) {



    return (
        <Box sx={{display: 'inline'}}>
            <FormGroup sx={{ display: 'inline' }}>
                <FormControlLabel control={<Checkbox id='customLighting' checked={props.initial} onClick={() => {props.onAllowUsage(!props.allowed);  props.onClick() }}/>} label={props.label} />
            </FormGroup>
        </Box>

    );


}



