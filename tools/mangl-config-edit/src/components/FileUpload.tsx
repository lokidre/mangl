import React from 'react';
import { Button, Stack, Typography } from '@mui/material';

import { requestProjectOpenDialog } from '../ipc/worker_api';


interface FileUploadProps {
    onLoadEnd: (name:string, data:any) => void;
}

export default function FileUpload(props: FileUploadProps) {
    const [loadedFilePath, setLoadedFilePath] = React.useState();
    const [loadedFileDir, setLoadedFileDir] = React.useState();
   
    return <Stack direction='row'>
        <Button variant='outlined' onClick={() => requestProjectOpenDialog()}>Open Project...</Button>
        <Typography>{loadedFilePath}</Typography>
    </Stack>
}

/*
export default function FileUpload(props: FileUploadProps) {

    const handleFileUpload = (event: React.ChangeEvent<HTMLInputElement>) => {
        const file = event.target.files?.[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (e) => {
                const content = e.target?.result as string;
                try {
                    props.onLoadEnd(file.name, content);
                } catch (error) {
                    console.error('Error parsing JSON:', error);
                }
            };
            reader.readAsText(file);
        }
    };

    return (
        <>
            <div>
                <input type='file' onChange={handleFileUpload} />
            </div>
        </>
    );
}
*/
