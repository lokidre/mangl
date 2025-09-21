import * as React from 'react';
import SwipeableViews from 'react-swipeable-views';
import {useTheme} from '@mui/material/styles';
import AppBar from '@mui/material/AppBar';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import Typography from '@mui/material/Typography';
import Box from '@mui/material/Box';
import {Button, Divider, List, ListItem, ListItemButton, ListItemIcon, ListItemText} from '@mui/material';
import * as GD from '../GlobalDispatch';

import InsertDriveFileIcon from '@mui/icons-material/InsertDriveFile';
import DeleteIcon from '@mui/icons-material/Delete';
import UploadPanel from './UploadPanel';
import {GlobalStateInstance} from '../GlobalStateProvider';

interface TabPanelProps {
    children?: React.ReactNode;
    dir?: string;
    index: number;
    value: number;
}

function TabPanel(props: TabPanelProps) {
    const {children, value, index, ...other} = props;

    return (
        <div
            role='tabpanel'
            hidden={value !== index}
            id={`full-width-tabpanel-${index}`}
            aria-labelledby={`full-width-tab-${index}`}
            {...other}
        >
            {value === index && (
                <Box sx={{p: 3}}>
                    <Typography component={'span'}>{children}</Typography>
                </Box>
            )}
        </div>
    );
}

function a11yProps(index: number) {
    return {
        id: `full-width-tab-${index}`,
        'aria-controls': `full-width-tabpanel-${index}`,
    };
}

export default function FileTabs() {
    // const global = React.useContext(GlobalStateInstance).state
    const theme = useTheme();
    const [value, setValue] = React.useState(0);
    const [_, forceUpdate] = React.useReducer((x) => x + 1, 0);

    const [files, setFiles] = React.useState<any | null>(null);

    // let files = global.files

    // setFiles(JSON.parse(window.localStorage.getItem('files')))

    React.useEffect(() => {
        const storageListener = () => {
            let storedData = window.localStorage.getItem('files');
            if (storedData) {
                setFiles(JSON.parse(storedData));
            }

            forceUpdate();
        };

        GD.pubsub_.addListener(GD.storageListenerId, storageListener);

        let storedData = window.localStorage.getItem('files');
        if (storedData) {
            setFiles(JSON.parse(storedData));
        }


        return () => GD.pubsub_.removeListener(GD.storageListenerId, storageListener);
    }, []);

    // let files = JSON.parse(window.localStorage.getItem('files'));
    // console.log('got file');

    // console.log('FILES', files);

    // const refresh = () => window.location.reload();

    const deleteFile = (fileName) => {
        for (let file of files) {
            if (file.name === fileName) {
                let remove_index = files.indexOf(file);
                files.splice(remove_index, 1);
                window.localStorage.setItem('files', JSON.stringify(files));
                forceUpdate();
            }
        }
    };

    // const TabTwo = (
    //   <div>
    //     <List>
    //       {files.map((text, index) => (
    //         <ListItem key={text.name} disablePadding>
    //           <ListItemIcon >
    //             <InsertDriveFileIcon />
    //           </ListItemIcon>
    //           <ListItemButton>
    //             <ListItemText primary={text.name} />
    //           </ListItemButton>
    //           <Button onClick={() => deleteFile(text.name)}>
    //             <ListItemIcon >
    //               < DeleteIcon />
    //             </ListItemIcon>
    //           </Button>
    //         </ListItem>
    //       ))}
    //     </List>
    //     <UploadPanel tabIndex={value} />
    //   </div>
    // );

    const TabOne = (
            <List>
                {files && files.map(function (file, index) {
                    if (file.tab === 0) {
                        return (
                            <ListItem key={file.name} disablePadding>
                                <ListItemIcon>
                                    <InsertDriveFileIcon />
                                </ListItemIcon>
                                <ListItemButton>
                                    <ListItemText>
                                        {file.name.length > 30 ? `${file.name.substring(0, 30)}...` : file.name}
                                    </ListItemText>
                                </ListItemButton>
                                <Button onClick={() => deleteFile(file.name)}>
                                    <ListItemIcon>
                                        <DeleteIcon />
                                    </ListItemIcon>
                                </Button>
                            </ListItem>
                        );
                    } else {
                        return '';
                    }
                })}
            </List>
    );

    const TabTwo = (
        <div>
            <List>
                {files && files.map(function (file, index) {
                    if (file.tab === 1) {
                        return (
                            <ListItem key={file.name} disablePadding>
                                <ListItemIcon>
                                    <InsertDriveFileIcon />
                                </ListItemIcon>
                                <ListItemButton>
                                    <ListItemText>
                                        {file.name.length > 30 ? `${file.name.substring(0, 30)}...` : file.name}
                                    </ListItemText>
                                </ListItemButton>
                                <Button onClick={() => deleteFile(file.name)}>
                                    <ListItemIcon>
                                        <DeleteIcon />
                                    </ListItemIcon>
                                </Button>
                            </ListItem>
                        );
                    } else {
                        return '';
                    }
                })}
            </List>
        </div>
    );

    const TabThree = (
            <List>
                {files && files.map(function (file, index) {
                    if (file.tab === 2) {
                        return (
                            <ListItem key={file.name} disablePadding>
                                <ListItemIcon>
                                    <InsertDriveFileIcon />
                                </ListItemIcon>
                                <ListItemButton>
                                    <ListItemText>
                                        {file.name.length > 30 ? `${file.name.substring(0, 30)}...` : file.name}
                                    </ListItemText>
                                </ListItemButton>
                                <Button onClick={() => deleteFile(file.name)}>
                                    <ListItemIcon>
                                        <DeleteIcon />
                                    </ListItemIcon>
                                </Button>
                            </ListItem>
                        );
                    } else {
                        return '';
                    }
                })}
            </List>
    );

    const handleChange = (event: React.SyntheticEvent, newValue: number) => {
        setValue(newValue);
    };

    const handleChangeIndex = (index: number) => {
        setValue(index);
    };

    return (
        <Box sx={{width: '100%'}}>
            <AppBar position='static'>
                <Tabs
                    value={value}
                    onChange={handleChange}
                    indicatorColor='secondary'
                    textColor='inherit'
                    variant='fullWidth'
                    aria-label='full width tabs example'
                >
                    <Tab label='Background' {...a11yProps(0)} />
                    <Tab label='Animation' {...a11yProps(1)} />
                    <Tab label='Model' {...a11yProps(2)} />
                </Tabs>
            </AppBar>
            <SwipeableViews
                axis={theme.direction === 'rtl' ? 'x-reverse' : 'x'}
                index={value}
                onChangeIndex={handleChangeIndex}
            >
                <TabPanel value={value} index={0} dir={theme.direction}>
                    {TabOne}
                </TabPanel>
                <TabPanel value={value} index={1} dir={theme.direction}>
                    {TabTwo}
                </TabPanel>
                <TabPanel value={value} index={2} dir={theme.direction}>
                    {TabThree}
                </TabPanel>
            </SwipeableViews>
            <Box sx={{mt: 3, textAlign: 'center'}}>
                <UploadPanel tabIndex={value} />
            </Box>
        </Box>
    );
}
