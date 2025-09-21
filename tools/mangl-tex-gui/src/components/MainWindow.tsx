import React from 'react';

import {Box, Grid, Stack, TextField, Typography} from '@mui/material';
import MyTable from './MyTable';

export default function MainWindow() {
    // <Greetings />
    return (
        <Box
            sx={{
                minHeight: '100vh',
            }}
        >
            <MyTable/>
        </Box>
    );
}
