import React, { useState } from 'react';
import { Button, Dialog, DialogActions, DialogContent, DialogTitle,} from '@mui/material';
import DeleteForeverIcon from "@mui/icons-material/DeleteForever";

interface DeletePopupProps {
  onDelete: () => void;
}

export default function DeletePopup(props:DeletePopupProps){
  const [open, setOpen] = useState(false);

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  const handleDelete = () => {
    handleClose();
    props.onDelete();
  };

  return (
    <div>
      <Button variant="contained" color="error" onClick={handleClickOpen}>
      <DeleteForeverIcon />
      </Button>
      <Dialog  open={open}  onClose={handleClose}  aria-labelledby="alert-dialog-title"  aria-describedby="alert-dialog-description">
        <DialogTitle id="alert-dialog-title">
         Delete Item ?
        </DialogTitle>
        <DialogContent>
            This action cannot be undone.
        </DialogContent>
        <DialogActions>
          <Button onClick={handleClose}>Cencel</Button>
          <Button onClick={handleDelete} autoFocus>
            Delete
          </Button>
        </DialogActions>
      </Dialog>
    </div>
  );
};
