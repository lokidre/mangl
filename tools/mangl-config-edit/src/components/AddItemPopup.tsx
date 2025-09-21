import React, { useState } from 'react';
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, TextField, List, ListItem, ListItemText, IconButton } from '@mui/material';
import DeleteIcon from '@mui/icons-material/Delete';   
import AddIcon from '@mui/icons-material/Add';
import AddItem from './AddItem';
import AddSection from './AddSection';


// interface Item {
//   name: string;
//   description: string;
//   options: string[];
// }


interface AddItemPopupProps{
    onSave: (el:any) => void
    type: 'el' | 'sec',
}
export default function AddItemPopup(props:AddItemPopupProps){

  const [open, setOpen] = useState(false);
  const [obj, setObj] = useState();

//   const [name, setName] = useState('');
//   const [description, setDescription] = useState('');
//   const [options, setOptions] = useState<string[]>([]);

  


  function onAddedElemChange(newEl:any){
    console.log('onAddedElemChange', newEl)
    setObj(newEl)
    props.onSave(newEl)
    setOpen(false); 
  };

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  return (
    <div>
      <Button variant="outlined" color="primary" onClick={handleClickOpen}> Add {props.type} <AddIcon/></Button>
      <Dialog open={open} onClose={handleClose} aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">Add</DialogTitle>

        <DialogContent>
          {(props.type === 'el' ) ? 
          (<AddItem  onChange={onAddedElemChange}/>) 
          : 
          (<AddSection  onChange={onAddedElemChange}/>)
          }
            
        </DialogContent>

        <DialogActions>
          <Button onClick={handleClose} color="primary"> Cancel </Button>
        </DialogActions>

      </Dialog>
    </div>
  );
}