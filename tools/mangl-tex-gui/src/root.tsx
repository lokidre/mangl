import React from 'react';
import {createRoot} from 'react-dom/client';

import {GlobalStyles} from '@mui/material';
import {CssBaseline, ThemeProvider} from '@mui/material';

import {muiTheme_, globalStyles} from './theme';

import MainWindow from './components/MainWindow';

const container = document.getElementById('root')!;
const root = createRoot(container);

const inputGlobalStyles = <GlobalStyles styles={globalStyles} />;

root.render(
    <ThemeProvider theme={muiTheme_}>
        {inputGlobalStyles}
        <CssBaseline />
        <MainWindow />
    </ThemeProvider>
);
