import {createTheme} from '@mui/material/styles';

class ThemeBase {
    screenButtonWidth = 140;
    dialogInputWidth = 100;

    panelBorderWidth = 1;
    panelBorderStyle = 'solid';

    sectionBorderWidth = 1;
    sectionBorderStyle = 'solid';
    sectionBorderRadius = 1;
}

class LiteTheme extends ThemeBase {
    //backgroundColor = '#25afce';
    backgroundColor = '#F5F5F5'; // White Smoke
    dialogBackgroundColor = '#ffffff';

    textColor = '#111111';
    busyTextColor = '#996515';
    errorTextColor = '#dc143c';
    //valueColor = '#11aa11';
    valueColor = '#1976d2'; // Default MUI color

    panelBorderColor = '#aaaaaa';
    sectionBorderColor = '#aaaaaa';
    sectionLabelColor = '#888888';
}

class DarkTheme extends ThemeBase {
    //backgroundColor = '#25afce';
    backgroundColor = '#2d2d30';
    dialogBackgroundColor = '#383838';

    // "#9EEAF9"
    textColor = '#eeeeee';
    valueColor = '#4caf50';

    busyTextColor = '#ffeb3b';
    errorTextColor = '#ff3d00';

    panelBorderColor = 'rgba(255, 255, 255, 0.12)';
    sectionBorderColor = 'rgba(255, 255, 255, 0.12)';
    sectionLabelColor = '#888888';
}

const darkMode = true;

//export const theme_ = new DarkTheme();
export const theme_ = darkMode ? new DarkTheme() : new LiteTheme();

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
    // "*": {
    //     draggable: false,
    // },

    body: {
        fontFamily: globalFonts.join(','),

        fontSmooth: 'auto',
        WebkitFontSmoothing: 'antialiased',
        MozOsxFontSmoothing: 'grayscale',

        minHeight: '100vh',
        // display: 'flex',
        // flex: 1,
    },

    '#root': {
        minHeight: '100vh',
    },

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
        background: {
            default: theme_.backgroundColor,
        },
    },

    typography: {
        fontWeightMedium: 500,
        fontSize: 18,

        h1: {
            fontSize: '2.2rem',
            fontWeight: 400,
            //color: theme_.textColor,
        },

        h2: {
            fontSize: '2.2rem',
            fontWeight: 400,
            //color: theme_.textColor,
        },

        // body1: {
        //     fontSize: '1rem',
        //     color: theme_.textColor,
        // },

        // body2: {
        //     fontSize: '1rem',
        //     color: theme_.valueColor,
        // },
    },
});

//export default theme;
