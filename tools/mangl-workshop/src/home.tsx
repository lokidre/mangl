import * as React from 'react';
import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import Divider from '@mui/material/Divider';
import Drawer from '@mui/material/Drawer';
import IconButton from '@mui/material/IconButton';

import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemButton from '@mui/material/ListItemButton';
import ListItemIcon from '@mui/material/ListItemIcon';
import ListItemText from '@mui/material/ListItemText';

import InsertDriveFileIcon from '@mui/icons-material/InsertDriveFile';
import MenuIcon from '@mui/icons-material/Menu';


import Toolbar from '@mui/material/Toolbar';
import Typography from '@mui/material/Typography';
import FullWidthTabs from 'components/Tabs';
import { Stack } from '@mui/system';
import FilePciker from './components/FilePicker';
import ViewBox from './components/ViewBox';
import Settings from './components/Settings';
import Grid from '@mui/material/Grid';
import { GlobalStateDefValue, GlobalStateInstance } from './GlobalStateProvider';


const drawerWidth = '15%';



export default function HomePage() {

    const [global, setGlobal] = React.useState(GlobalStateDefValue.state)


    return (
    <GlobalStateInstance.Provider value={{state: global, setState: setGlobal}}>
        <Grid container>
        <Grid item xs={3}>
            <FilePciker />
        </Grid>
        <Grid item xs={6}>
            <ViewBox />
        </Grid>
        <Grid item xs={3}>
            <Settings />
        </Grid>
    </Grid>
    </GlobalStateInstance.Provider>

    );
}