import * as React from 'react';
import Dialog from '@mui/material/Dialog';
import DialogTitle from '@mui/material/DialogTitle';
import DialogContent from '@mui/material/DialogContent';
import DialogActions from '@mui/material/DialogActions';
import TextField from '@mui/material/TextField';
import Button from '@mui/material/Button';

interface PathDialogProps{
  onSubmit: (path:string)=>void
}

export default function PathDialog(props:PathDialogProps) {
  const [open, setOpen] = React.useState(false);
  const [textFieldValue, setTextFieldValue] = React.useState('');

  const handleClickOpen = () => {
    setOpen(true);
  };
  const handleSubmit = () => {
    props.onSubmit(textFieldValue)
    handleClose()
  };


  const handleClose = () => {
    setOpen(false);
  };

  const handleTextFieldChange = (event:any) => {
    setTextFieldValue(event.target.value);
  };

  return (
    <div>
      <Button variant="outlined" onClick={handleClickOpen}>
       Settings
      </Button>
      <Dialog open={open} onClose={handleClose}>
        <DialogTitle>Props</DialogTitle>
        <DialogContent>
          <TextField
            autoFocus
            margin="dense"
            id="name"
            label="Enter programm path"
            type="text"
            fullWidth
            value={textFieldValue}
            onChange={handleTextFieldChange}
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={handleClose}>Cancel</Button>
          <Button onClick={handleSubmit}>Submit</Button>
        </DialogActions>
      </Dialog>
    </div>
  );
}
