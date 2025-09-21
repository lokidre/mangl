import React, { useState } from 'react';
import { TextField, Button, List, ListItem, ListItemText, IconButton } from '@mui/material';
import DeleteIcon from '@mui/icons-material/Delete';

interface AddItemProps{
    onChange: (el:any)=> void
}

  
function validateSelection(selection:string[], value:string){
    let tmpArr:string[] = JSON.parse(JSON.stringify(selection))

    if(tmpArr.length > 0){
        tmpArr = tmpArr.map((el:string, i:number)=>{
            if( !(el.trim().length === 0)){ //ete datark a , kam menak space a 
              return el
            }
        })
        if(!tmpArr.includes(value)){
            tmpArr.push(value)
        }
    }
    return tmpArr
}



export default function AddItem(props:AddItemProps){
  const [name, setName] = useState('');
  const [value, setValue] = useState('');
  const [description, setDescription] = useState('');
  const [selection, setSelection] = useState<string[]>([]);

  const handleAddOption = () => {
    setSelection([...selection, '']);
  };

  const handleRemoveOption = (index: number) => {
    setSelection(selection.filter((_, i) => i !== index));
  };

  const handleSubmit = () => {
    if(! (name && value) ){ return }

    let newItem = {
        name,
        value,
        description,
        selection: validateSelection(selection, value),
    }
    props.onChange(newItem)
  };

  return (
    <div>
      <TextField label="Name" value={name} onChange={(e) => setName(e.target.value)} />
      <br />
      <TextField label="Value" value={value} onChange={(e) => setValue(e.target.value)} />
      <br />
      <TextField label="Description" value={description} onChange={(e) => setDescription(e.target.value)} />
      <br />
      <List>
        {selection.map((option, index) => (
          <ListItem key={index}>
            <ListItemText primary={<TextField value={option} onChange={(e) => {
              const updatedOptions = [...selection];
              updatedOptions[index] = e.target.value;
              setSelection(updatedOptions);
            }} fullWidth />}> </ListItemText>
              <IconButton edge="end" aria-label="delete" onClick={() => handleRemoveOption(index)}>
                <DeleteIcon />
              </IconButton>
          </ListItem>
        ))}
      </List>
      <Button variant="contained" color="primary" onClick={handleAddOption}>Add Option</Button>
      <br />
      <Button variant="contained" color="primary" onClick={handleSubmit}>Submit</Button>
    </div>
  );
};
