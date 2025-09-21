import * as React from 'react';
import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';



import FilePciker from './components/FilePicker';
import ViewBox from './components/ViewBox';
import Settings from './components/Settings';
import Grid from '@mui/material/Grid';
import { GlobalStateDefValue, GlobalStateInstance } from 'GlobalStateProvider';
// import { GlobalStateDefValue, GlobalStateInstance } from './GlobalStateProvider';


const drawerWidth = '15%';



export default function HomePage() {

    // const [global, setGlobal] = React.useState(GlobalStateDefValue.state)
    const [stateCtx, setCtx] = React.useState(GlobalStateDefValue.state)

    React.useEffect(() => {
        // Read from local storage

    }, []);


    

    return (
        <GlobalStateInstance.Provider value={{state: stateCtx, setState: setCtx}}>
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
