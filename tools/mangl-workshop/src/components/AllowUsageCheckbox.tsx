import * as React from 'react';
import { Container, Box, Typography, Popover, Button, FormGroup, FormControlLabel, Checkbox } from "@mui/material";


interface allowUsageProps {
    label: string,
    allowed: boolean,
    onAllowUsage: (allowed: boolean) => void;

}


export default function AllowUsageCheckbox(props: allowUsageProps) {



    return (
        <Box>
            <FormGroup sx={{ display: 'inline' }}>
                <FormControlLabel control={<Checkbox id='customLighting'  onClick={() => props.onAllowUsage(!props.allowed)}/>} label={props.label} />
            </FormGroup>
        </Box>

    );


}



