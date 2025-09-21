import * as React from 'react';
import { useTheme } from '@mui/material/styles';
import AppBar from '@mui/material/AppBar';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import Typography from '@mui/material/Typography';
import Box from '@mui/material/Box';
import { Button, Checkbox, Divider, FormControlLabel, FormGroup, List, ListItem, ListItemButton, ListItemIcon, ListItemText } from '@mui/material';
import * as GD from '../GlobalDispatch';

import InsertDriveFileIcon from '@mui/icons-material/InsertDriveFile';
import DeleteIcon from '@mui/icons-material/Delete';
import UploadPanel from './UploadPanel';
import EditIcon from '@mui/icons-material/Edit';
import { GlobalStateInstance } from 'GlobalStateProvider';



// import {GlobalStateInstance} from '../GlobalStateProvider';

interface TabPanelProps {
    children?: React.ReactNode;
    dir?: string;
    index: number;
    value: number;
}

function TabPanel(props: TabPanelProps) {
    const { children, value, index, ...other } = props;



    return (
        <div
            role='tabpanel'
            hidden={value !== index}
            id={`full-width-tabpanel-${index}`}
            aria-labelledby={`full-width-tab-${index}`}
            {...other}
        >
            {value === index && (
                <Box sx={{ p: 3 }}>
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
    const stateCtx = React.useContext(GlobalStateInstance).state;
    const theme = useTheme();
    const [value, setValue] = React.useState(0);
    const [_, forceUpdate] = React.useReducer((x) => x + 1, 0);

    // const [files, setFiles] = React.useState<any | null>(null);
    const file_tabs = ['backgrounds', 'animations', 'models', 'blueprints']
    stateCtx.currentTab = file_tabs[value]
    const [backgrounds, setBackgrounds] = React.useState<any | null>(null);
    const [animations, setAnimations] = React.useState<any | null>(null);
    const [models, setModels] = React.useState<any | null>(null);
    const [blueprints, setBlueprints] = React.useState<any | null>(null);




    // let files = global.files

    // setFiles(JSON.parse(window.localStorage.getItem('files')))

    React.useEffect(() => {
        const storageListener = () => {
            let storedData = JSON.parse(window.localStorage.getItem(stateCtx.currentTab));
            // let data =localStorage.getItem(current_tab);
            // if (storedData) {
            //     setFiles(JSON.parse(storedData));
            // }
            switch (stateCtx.currentTab) {
                case file_tabs[0]: {
                    setBackgrounds(storedData);
                    break;
                }
                case file_tabs[1]: {
                    setAnimations(storedData);
                    break;
                }
                case file_tabs[2]: {
                    setModels(storedData);
                    break;
                }
                case file_tabs[3]: {
                    setBlueprints(storedData);
                    break;
                }

            }

            forceUpdate();
        };

        GD.pubsub_.addListener(GD.storageListenerId, storageListener);

        // let storedData = window.localStorage.getItem('files');
        // if (storedData) {
        //     setFiles(JSON.parse(storedData));
        // }


        setBackgrounds(stateCtx.backgrounds);
        setAnimations(stateCtx.animations);
        setModels(stateCtx.models);
        setBlueprints(stateCtx.blueprints);



        return () => GD.pubsub_.removeListener(GD.storageListenerId, storageListener);
    }, []);

    // let files = JSON.parse(window.localStorage.getItem('files'));
    // console.log('got file');

    // console.log('FILES', files);

    // const refresh = () => window.location.reload();

    const deleteFile = (fileName) => {

        let search_tab = backgrounds;

        switch (stateCtx.currentTab) {
            case file_tabs[0]: {
                search_tab = backgrounds;
                break;
            }
            case file_tabs[1]: {
                search_tab = animations;
                break;
            }
            case file_tabs[2]: {
                search_tab = models;
                Module.call('deleteModel', '')
                break;
            }
            case file_tabs[3]: {
                search_tab = blueprints;
                break;
            }
        }

        for (let file of search_tab) {
            if (file.name === fileName) {
                let remove_index = search_tab.indexOf(file);
                search_tab.splice(remove_index, 1);
                localStorage.setItem(stateCtx.currentTab, JSON.stringify(search_tab));
                if (stateCtx.currentFile.name === fileName) {
                    stateCtx.currentFile = {}
                    localStorage.setItem('currentFile', '{}');
                }
                forceUpdate();

            }
        }
    };


    const fileSelect = (file: any) => {

        let currentFile = { file: file.file, name: file.name, callFunction: '' }

        // console.log('type of currentFile ', typeof(stateCtx.currentFile.toString())


        switch (stateCtx.currentTab) {
            case file_tabs[0]:
                Module.call('setBackground', file.file)
                currentFile.callFunction = 'setBackground'
                break

            case file_tabs[1]:
                Module.call('deleteModel', '')
                Module.call('setAnimation', file.file)
                currentFile.callFunction = 'setAnimation'
                break

            case file_tabs[2]:
                Module.call('setModel', file.file)
                currentFile.callFunction = 'setModel'
                stateCtx.loadModelProperties()
                break

            case file_tabs[3]:
                Module.call('setBlueprint', file.file)
                currentFile.callFunction = 'setBlueprint'
                break
        }

        stateCtx.currentFile = currentFile
        localStorage.setItem('currentFile', JSON.stringify(currentFile))

        // window.location.reload()
    };


    const TabOne = (
        <List>
            {backgrounds && backgrounds.map(function (file, index) {

                return (
                    <ListItem key={file.name} disablePadding>
                        <ListItemIcon>
                            <InsertDriveFileIcon />
                        </ListItemIcon>
                        <ListItemButton onClick={() => fileSelect(file)}>
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

            })}
        </List>
    );

    const TabTwo = (
        <List>
            {animations && animations.map(function (file, index) {

                return (
                    <ListItem key={file.name} disablePadding>
                        <ListItemIcon>
                            <InsertDriveFileIcon />
                        </ListItemIcon>
                        <ListItemButton onClick={() => fileSelect(file)}>
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

            })}
        </List>
    );

    const TabThree = (
        <List>
            {models && models.map(function (file, index) {

                return (
                    <ListItem key={file.name} disablePadding>
                        <ListItemIcon>
                            <InsertDriveFileIcon />
                        </ListItemIcon>
                        <ListItemButton onClick={() => fileSelect(file)}>
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

            })}
        </List>
    );


    const TabFour = (
        <List>
            {blueprints && blueprints.map(function (file, index) {

                return (
                    <ListItem key={file.name} disablePadding>
                        <ListItemIcon>
                            <InsertDriveFileIcon />
                        </ListItemIcon>
                        <ListItemButton onClick={() => fileSelect(file)}>
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
        <Box sx={{ width: '100%' }}>
            <AppBar position='static'>
                <Tabs
                    value={value}
                    onChange={handleChange}
                    textColor='inherit'
                    variant='fullWidth'
                    aria-label='full width tabs example'
                >
                    <Tab label='Background' {...a11yProps(0)} />
                    <Tab label='Animation' {...a11yProps(1)} />
                    <Tab label='Model' {...a11yProps(2)} />
                    <Tab label='Blueprint' {...a11yProps(3)} />
                </Tabs>
            </AppBar>
            <TabPanel value={value} index={0} dir={theme.direction}>
                {TabOne}
            </TabPanel>
            <TabPanel value={value} index={1} dir={theme.direction}>
                {TabTwo}
            </TabPanel>
            <TabPanel value={value} index={2} dir={theme.direction}>
                {TabThree}
            </TabPanel>
            <TabPanel value={value} index={3} dir={theme.direction}>
                {TabFour}
            </TabPanel>
            <Box sx={{ mt: 3, textAlign: 'center' }}>
                <UploadPanel current_tab={stateCtx.currentTab} />
            </Box>
        </Box>
    );
}
