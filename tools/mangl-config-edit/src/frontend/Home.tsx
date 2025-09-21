import { Button, List, ListItem, Checkbox, Typography, Box, Stack} from '@mui/material';
import {useState} from 'react';
import React from 'react';
import FileUpload from '../components/FileUpload';
import ListElement from '../components/ListElement';
import * as Wrk from '../ipc/worker_api';
import * as GD from '../ipc/gd';
import * as ID from '../ipc/worker_ids';
import {getAddedProps, downloadFile, modExportFile, modUplData, VarObj, YAMLtoJSON, genEncrFileContent, serverYmlDataToClient} from '../lib/modifyData';
import PathDialog from '../components/PathDialog';
import AddItemPopup from '../components/AddItemPopup';
import { Project } from '../setup/project';
import { ProjectStateDefValue } from './ProjectStateProvider';
import { YamlSection } from '../lib/yamlPackageClasses';

export default function Home() {
    //const [projectCtx, setScanCtx] = React.useState(ScanViewStateDefValue.state);

    const projectCtxRef = React.useRef(ProjectStateDefValue.state);


    const [uplData, setUplData] = useState<any>([]);
    const [fileName, setFileName] = useState('');

    React.useEffect(() => {
        console.log('uplData', uplData);
    }, [uplData]);

    React.useEffect(() => {
        const respondFileData = (data: Project) => {
            projectCtxRef.current.project = data;
            projectCtxRef.current.loaded = true;
            projectCtxRef.current.unsaved = false;
            // console.log(data.configObject)
            data.configObject = serverYmlDataToClient(data.configObject);
            // console.log(projectCtxRef.current);
            // console.log(data.configObject.config);
            //GD.pubsub_.removeListener(ID.RespondFileDataId, respondFileData);

            setUplData(data.configObject.config.sections)
        };

        GD.pubsub_.addListener(ID.OnProjectLoadedId, respondFileData);
    }, []);

    const onLoadEnd = (name: string, data: any) => {
        setFileName(name);
        Wrk.requestFileData(data); //uxarkum a mer filei mijin@
    };

    const handleSaveFile = () => {
        let resArr = modExportFile(uplData);
        downloadFile(resArr, fileName, 'yaml');
        
        resArr = genEncrFileContent(uplData);
        downloadFile(resArr, fileName, 'txt');

    };
    

    const saveEditEl = (obj: VarObj) => {
        uplData.forEach((sec:any)=>{
            sec.properties?.forEach((el:VarObj)=>{
                if(el.id === obj.id){
                    el.name = obj.name
                    el.value = obj.value
                    el.description = obj.description
                    el.selection = obj.selection
                }
            })
        })
        setUplData([...uplData]);
        console.log('element saved', uplData);
    };

    const saveNewElem = (sec:any, newEl:any) => {
        let addedProps  = getAddedProps()

        if (newEl.selection?.length) {
            addedProps = {...addedProps, ...{type: 'selection'}}
        } else{
            addedProps = {...addedProps, ...{type: 'string'}}
        }

        let modEl: VarObj = { ...addedProps, ...newEl};

        let tmpArr = JSON.parse(JSON.stringify(uplData))
        tmpArr.forEach((s:any)=>{ 
            if(s.name === sec.name){
                s.properties.push(modEl)
                // s.properties?.forEach((el:any, i:number)=>{
                //     if(el.id === id){
                //         s.properties.splice(i, 0, modEl);
                //     }
                // })
            }
        })
        setUplData(tmpArr)

        // setUplData([...uplData]);
    };

    const saveNewSec = (sec:any) => {
        let tmpSec = new YamlSection(sec.name, sec.description);

        setUplData([...uplData, tmpSec])
    };


    const deleteElem = (obj: VarObj) => {
        uplData.forEach((sec:any)=>{
            sec.properties?.forEach((el:VarObj, i:number)=>{
                if(el.id === obj.id){
                    sec.properties.splice(i, 1)  
                }
            })
        })
        setUplData([...uplData]);                 
    };

    return (
        <Stack component='main'>
            <Typography> If your variable exist in harcoded section, but is not hardcoded variable it will be moved to OTHER section</Typography>
            <PathDialog onSubmit={(path: string) => { console.log("not implemented");}} />

            {fileName && ( <> <Button variant={'contained'} color='warning' onClick={handleSaveFile}> SAVE file </Button> </> )}


            <List>
                {uplData.length ? (
                    <>
                    {uplData.map((sec: any, i: number) => (
                        <Box sx={{padding:'15px 0 '}} key={i}>
                            <Typography variant='h6'>{sec.name}, 
                                <Typography sx={{fontSize:'10px'}}> {sec.description} </Typography>
                            </Typography>

                            {
                                (sec.properties && sec.properties.length) ? 
                                <>
                                    <ul>
                                        {sec.properties.map((el: VarObj,) => (
                                            <ListItem key={el.id}>
                                                <ListElement obj={el} saveElEdit={saveEditEl} delEl={deleteElem} />
                                            </ListItem>
                                        ))}
                                    </ul>
                                </>
                                : 
                                <> Section is empty </> 
                            }
                            {
                                sec.custom && <AddItemPopup type='el' onSave={(newEl:any)=> saveNewElem(sec, newEl)}/>}
                        </Box>
                    ))}
                    {<AddItemPopup type='sec' onSave={(sec)=>{saveNewSec(sec)}}/>}
                    </>
                ) : (
                    
                    <>
                        <FileUpload onLoadEnd={onLoadEnd} />
                        No File Uploaded, please upload Yaml
                    </>
                )}
            </List>
        </Stack>
    );
}
