
import React, { useState } from "react";
import { Button } from "@mui/material";
// import { GlobalStateInstance } from "../GlobalStateProvider";
import * as GD from "../GlobalDispatch";
import { Interface } from "readline";


// const files: any[] = [];
// window.localStorage.setItem("files", JSON.stringify([]));


// class UploadPanel extends React.Component<{ tabIndex: number }, {}>{


//     constructor(props) {
//         super(props);
//         this.state = {
//             files: []
//         };
//         this.convert = this.convert.bind(this);
//     }


//     convert(e) {
//         // check max. file size is not exceeded
//         // size is in bytes
//         if (e.target.files[0].size > 2000000) {
//             console.log("File too large");
//             return;
//         }
//         var reader = new FileReader();

//         // reader.readAsDataURL(e.target.files[0]); || for base_64
//         reader.readAsText(e.target.files[0]);

//         const refresh = () => window.location.reload()


//         console.log('PROPS', this.props)

//         reader.onload = () => {
//             console.log(reader.result); //base64encoded string

//             // if (reader.result != null) {
//             //     const base64String = reader.result
//             //         .replace('data:', '')
//             //         .replace(/^.+,/, '');
//             // }


//             let fileObj = {
//                 name: e.target.files[0].name,
//                 timestamp: Date.now(),
//                 // file_base64: reader.result || this is for base_64,
//                 file: reader.result,
//                 tab: this.props.tabIndex
//             };


//             files.push(fileObj)
//             global.files = files
//             GD.pubsub_.dispatch(GD.storageListenerId, global.files)


//             // refresh()
//         };
//         reader.onerror = error => {
//             console.log("Error: ", error);
//         };




//     }
//     override render() {
//         return (
//             <>
//                 <input
//                     accept="image/*"
//                     style={{
//                         display: "none"
//                     }}
//                     id="button-file"
//                     type="file"
//                     onChange={this.convert}
//                 />
//                 <label htmlFor="button-file">
//                     <Button
//                         variant="contained"
//                         color="primary"
//                         component="span"
//                     >
//                         Upload File
//                     </Button>
//                 </label>
//             </>
//         );
//     }
// }

// export default UploadPanel;

interface props {
    current_tab: string
}


export default function UploadPanel(props: props) {

    // const files = JSON.parse(window.localStorage.getItem('files'))
    // console.log('opened')

    // const global = React.useContext(GlobalStateInstance).state
    // global.files = files


    // const [file, setFile] = useState()

    let ext = ''

    switch(props.current_tab) { 

        case 'backgrounds': { 
            ext = 'image/png'; 
           break; 
        } 
        case 'animations': { 
            ext = '.ini'; 
           break; 
        } 
        case 'models' : { 
            ext = '.obj'; 
           break; 
        } 
        case 'blueprints' : { 
            ext = 'image/png'; 
           break; 
        } 
        default: { 
           ext = 'all'; 
           break; 
        } 
     } 


    const handleChange = (event) => {
        let file = event.target.files[0]


        let reader = new FileReader();


        reader.onloadend = function (e) {
            let fileObj = {
                name: file.name,
                timestamp: Date.now(),
                // file_base64: reader.result || this is for base_64,
                file: reader.result,
                // tab: props.tabIndex
            }

            let storedFiles = localStorage.getItem(props.current_tab)
            console.log('GOt it')
            let files = storedFiles ? JSON.parse(storedFiles) : new Array<any>();
            
            // global.files = files
            files.push(fileObj)
            localStorage.setItem(props.current_tab, JSON.stringify(files))
            GD.pubsub_.dispatch(GD.storageListenerId, files)
        }

        reader.readAsText(file)






    }



    return (
        <>
            <input
                style={{
                    display: "none"
                }}
                id="button-file"
                type="file"
                accept={ext}
                onChange={handleChange}
            />
            <label htmlFor="button-file">
                <Button
                    variant="contained"
                    color="primary"
                    component="span"
                >
                    Upload File
                </Button>
            </label>
        </>
    );
}







// import React, { useEffect, useState } from "react";

// import { Box, Button, Dialog, DialogTitle, Grid, Stack, Typography } from "@mui/material";
// import { styled as muiStyled } from '@mui/system';


// const ImageComponent = muiStyled('img')({});

// function isIMage(fileName) {
//     var fileExt = getFileExtension(fileName);
//     var imagesExtension = ['png', 'jpg', 'jpeg'];
//     if (imagesExtension.indexOf(fileExt) !== -1) {
//         return fileExt;
//     } else {
//         return false;
//     }
// }

// function getFileExtension(fileName) {
//     let fileExtension = fileName.replace(/^.*\./, '');
//     return fileExtension;
// }

// interface DesktopDisplayPanelProps {
//     tag: string
// }

// export default function UploadPanel(props: DesktopDisplayPanelProps) {


//     const [error, setError] = React.useState<Error | string | null>(null);


//     const imageExtRef = React.useRef('');


//     const [showImage, setShowImage] = React.useState('');


//     const [fileName, setFileName] = React.useState<string>('Image file is not selected.');
//     const filesRef = React.useRef<File[]>([]);
//     const fileNameRef = React.useRef('');
//     const inputFileRef: any = React.useRef(null);

//     const [wrongCardIdInput, setWrongCardIdInput] = React.useState(false);
//     const [warningMessage, setWarningMessage] = React.useState<string | null>(null);

//     const [progIndicator, setProgIndicator] = React.useState(false);
//     const [imageUploadingIndicator, setImageUploadingIndicator] = React.useState('');

//     const [items, setItems] = useState([]);


//     const errorFieldNames2 = React.useRef(new Map<string, any>());

//     const onClose = React.useCallback((event, reason) => {
//         if ((reason && reason === 'backdropClick') || 'escapeKeyDown') return;
//         // props.onClose();
//     }, []);


//     const onSelectedFileChange = React.useCallback((tag: string, e) => {
//         const files = e.target.files;
//         console.log(e.target.files)
//         console.log('tag', tag)

//         if (files && files.length) {
//             const fileType = files.type;
//             let file = files[0];

//             let imageExt = isIMage(file.name);
//             if (imageExt !== false) {
//                 imageExtRef.current = imageExt;

//                 const reader = new FileReader();

//                 reader.readAsDataURL(file);
//                 reader.addEventListener('load', () => {
//                 });

//                 filesRef.current = files;
//                 setFileName(file.name);

//                 console.log(file)


//                 localStorage.setItem('items', JSON.stringify('asd'));
//                 console.log('saved in local-stor')

//             } else {
//                 setWarningMessage('Wrong image file extention!');
//                 setWrongCardIdInput(true);
//             }
//         }
//     }, []);

//     const onBrowseClick = () => {
//         inputFileRef.current.click();
//     };

//     // const onUpload = React.useCallback((files: any, tag: string) => {
//     //     console.log(tag)
//     //     Api.requestSessionImage<any>(
//     //         'user',
//     //         'upload_profile_image',
//     //         {
//     //             imgName: files.name,
//     //             resourceTag: tag
//     //         },
//     //         files
//     //     )
//     //         .then(() => {
//     //             fileNameRef.current = files.name;
//     //             setShowImage(files.name);
//     //             setImageUploadingIndicator('');
//     //         })
//     //         .catch(setError);
//     // }, []);

//     // const onData = React.useCallback((apiData: Api.UserParam) => {

//     //     setDataReady(true);

//     // }, []);

//     const onRemove = () => {
//         let imageName: string;
//         if (showImage !== '') {
//             imageName = showImage;
//         } else {
//             imageName = fileNameRef.current;
//         }
//         setImageUploadingIndicator('Removing');
//     };

//     console.log('props.tag', props.tag)
//     console.log('showImage', showImage)

//     console.log('refrefref', fileNameRef)

//     return <Stack direction='column'>
//         {/* {imageUploadingIndicator !== '' && <ProgressIndicator title={imageUploadingIndicator} />}
//         {showImage !== '' && <ImageComponent sx={{ width: 200, height: 200, justifySelf: 'center', borderRadius: '50%' }} src={Env.getProfileImageLink(fileNameRef.current)} />} */}

//         {/* {showImage === '' && imageUploadingIndicator === '' && <ImageComponent sx={{ width: 200, height: 200, justifySelf: 'center', borderRadius: '50%' }} src={`${process.env.PUBLIC_URL}/images/upload-imag-1.jpg`} />} */}


//         <input
//             style={{ display: 'none' }}
//             ref={inputFileRef}
//             onChange={(e) => onSelectedFileChange(props.tag, e)}
//             type='file'
//         />

//         <Box sx={{ textAlign: 'center' }}>
//             <Button sx={{ width: '40%', margin: 2 }} variant='contained' onClick={onBrowseClick}>
//                 Upload File
//             </Button>



//             <Button sx={{ width: '40%', margin: 2 }} variant='contained' onClick={onRemove}>
//                 Remove file
//             </Button>
//         </Box>
//         {/* {progIndicator && (
//             <Dialog onClose={onClose} open={progIndicator} PaperProps={{ sx: { width: 200, height: 200 } }}>
//                 <ProgressIndicator title='Updating...' />
//             </Dialog>
//         )} */}

//     </Stack>
// }