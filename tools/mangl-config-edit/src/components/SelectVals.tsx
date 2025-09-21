import * as React from 'react';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import FormControl from '@mui/material/FormControl';
import Select, { SelectChangeEvent } from '@mui/material/Select';

interface SelectValsProps{
    // selections: any[], 
    required:boolean
    selections: any[], 
    value: any,
    onChange: (value:any)=>void
}

export default function SelectVals(props:SelectValsProps) {
  const [val, setVal] = React.useState(props.value);
  const [data, setdata] = React.useState(props.selections);

  React.useEffect(()=>{
    props.onChange(val)
  },[val])

  const handleChange = (event: SelectChangeEvent) => {
    setVal(event.target.value);
  };

  return (
    <FormControl sx={{ m: 1, minWidth: 120 }} size="small">
      <InputLabel id="demo-select-small-label">value</InputLabel>
      <Select
        labelId="demo-select-small-label"
        id="demo-select-small"
        value={val}
        label="Select Value"
        onChange={handleChange}
        error={props.required}
      >
        <MenuItem value={val}> {val} </MenuItem>
        
        {
          data.map((el:any, i:number)=>{ 
            return <MenuItem key={i} value={el}>{el}</MenuItem>
          })
        }
      </Select>
    </FormControl>
  );
}