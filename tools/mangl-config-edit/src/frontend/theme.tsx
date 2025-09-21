import {createTheme} from '@mui/material/styles';

class DarkTheme {
    backgroundColor = '#2d2d30';
};

const darkMode = true;

export const theme_ = new DarkTheme();

const globalFonts = [
    'Roboto',
    '-apple-system',
    'BlinkMacSystemFont',
    'Segoe UI',
    'Oxygen',
    'Ubuntu',
    'Cantarell',
    'Fira Sans',
    'Droid Sans',
    'Helvetica Neue',
    'sans-serif',
];

const codeFonts = ['source-code-pro', 'Menlo', 'Monaco', 'Consolas', 'Courier New', 'monospace'];

export const globalStyles = {
    "*": {
        draggable: false,
    },

    body: {
        fontFamily: globalFonts.join(','),

        fontSmooth: 'auto',
        WebkitFontSmoothing: 'antialiased',
        MozOsxFontSmoothing: 'grayscale',

        padding: '1em',

        //minHeight: '100vh',
        // display: 'flex',
        // flex: 1,
    },

    // '#root': {
    //     minHeight: '100vh',
    // },

    code: {
        fontFamily: codeFonts.join(','),
    },
};

export const muiTheme_ = createTheme({
    palette: {
        mode: darkMode ? 'dark' : 'light',

        // primary: {
        //     main: "#9EEAF9",
        // },
        // secondary: {
        //     main: "#9575CD",
        // },
        // background: {
        //     default: theme_.backgroundColor,
        // },
    },

    // typography: {
    //     fontWeightMedium: 500,
    //     fontSize: 18,

    //     h1: {
    //         fontSize: '2.2rem',
    //         fontWeight: 400,
    //         //color: theme_.textColor,
    //     },

    //     h2: {
    //         fontSize: '2.2rem',
    //         fontWeight: 400,
    //         //color: theme_.textColor,
    //     },

    // },
});
