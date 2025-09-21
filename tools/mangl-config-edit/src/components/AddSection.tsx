import React, { useState } from 'react';
import { TextField, Button, List, ListItem, ListItemText, IconButton } from '@mui/material';
import DeleteIcon from '@mui/icons-material/Delete';

interface AddSectionProps{
    onChange: (el:any)=> void
}



export default function AddSection(props:AddSectionProps){
  const [name, setName] = useState('');
  const [description, setDescription] = useState('');

  const handleSubmit = () => {
    if(!name ){ return }

    let newItem = {
        name,
        description,
    }
    props.onChange(newItem)
  };

  return (
    <div>
      <TextField label="Name" value={name} onChange={(e) => setName(e.target.value)} />
      <br />
      <TextField label="Description" value={description} onChange={(e) => setDescription(e.target.value)} />
      <br />
      <Button variant="contained" color="primary" onClick={handleSubmit}>Submit</Button>
    </div>
  );
};
