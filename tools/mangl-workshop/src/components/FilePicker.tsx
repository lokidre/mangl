import * as React from 'react';
import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import Drawer from '@mui/material/Drawer';
import FileTabs from './Tabs';
import { GlobalStateInstance } from '../GlobalStateProvider';

const drawerWidth = '25%';

interface Props {
    /**
     * Injected by the documentation to work in an iframe.
     * You won't need it on your project.
     */
    window?: () => Window;

}

export default function FilePicker(props: Props) {
    const { window } = props;
    const [mobileOpen, setMobileOpen] = React.useState(false);

    const global = React.useContext(GlobalStateInstance).state
    console.log('Height', global.canvasHeight)


    const container = window !== undefined ? () => window().document.body : undefined;



    return (
        <Box sx={{ display: 'flex' }}>


            <Drawer
                sx={{
                    width: drawerWidth,
                    flexShrink: 0,
                    '& .MuiDrawer-paper': {
                        width: drawerWidth,
                        boxSizing: 'border-box',
                    },
                }}
                variant="permanent"
                anchor="left"
            >
                <FileTabs />
            </Drawer>

        </Box>
    );
}



// import * as React from 'react';
// import AppBar from '@mui/material/AppBar';
// import Box from '@mui/material/Box';
// import CssBaseline from '@mui/material/CssBaseline';
// import Divider from '@mui/material/Divider';
// import Drawer from '@mui/material/Drawer';
// import IconButton from '@mui/material/IconButton';

// import List from '@mui/material/List';
// import ListItem from '@mui/material/ListItem';
// import ListItemButton from '@mui/material/ListItemButton';
// import ListItemIcon from '@mui/material/ListItemIcon';
// import ListItemText from '@mui/material/ListItemText';

// import InsertDriveFileIcon from '@mui/icons-material/InsertDriveFile';
// import MenuIcon from '@mui/icons-material/Menu';


// import Toolbar from '@mui/material/Toolbar';
// import Typography from '@mui/material/Typography';
// import FullWidthTabs from 'components/tabs';
// import { Stack } from '@mui/system';

// const drawerWidth = '30%';



// interface Props {
//     /**
//      * Injected by the documentation to work in an iframe.
//      * You won't need it on your project.
//      */
//     window?: () => Window;
// }

// export default function FilePicker(props: Props) {
//     const { window } = props;
//     const [mobileOpen, setMobileOpen] = React.useState(false);

//     const handleDrawerToggle = () => {
//         setMobileOpen(!mobileOpen);
//     };


//     const container = window !== undefined ? () => window().document.body : undefined;

//     return (
//         <Stack direction='column'>
//             <Box sx={{ display: 'flex'}}>
//                 <CssBaseline />
//                 <Box
//                     component="nav"
//                     sx={{ width: { sm: drawerWidth }, flexShrink: { sm: 0 } }}
//                     aria-label="mailbox folders"

//                 >
//                     {/* The implementation can be swapped with js to avoid SEO duplication of links. */}
//                     <Drawer
//                         container={container}
//                         variant="temporary"
//                         open={mobileOpen}
//                         onClose={handleDrawerToggle}
//                         ModalProps={{
//                             keepMounted: true, // Better open performance on mobile.
//                         }}
//                         sx={{
//                             display: { xs: 'block', sm: 'none' },
//                             '& .MuiDrawer-paper': { boxSizing: 'border-box', width: drawerWidth },
//                         }}
//                     >
//                        <FullWidthTabs />
//                        <Typography>asd</Typography>
//                     </Drawer>
//                     <Drawer
//                         variant="permanent"
//                         sx={{
//                             display: { xs: 'none', sm: 'block' },
//                             '& .MuiDrawer-paper': { boxSizing: 'border-box', width: drawerWidth },
//                         }}
//                         open
//                     >
//                        <FullWidthTabs />
//                     </Drawer>
//                 </Box>
//             </Box>
//         </Stack>
//     );
// }