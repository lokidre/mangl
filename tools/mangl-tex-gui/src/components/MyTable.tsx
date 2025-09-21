import * as React from 'react';
import Box from '@mui/material/Box';
import { DataGrid, GridColDef, GridValueGetterParams } from '@mui/x-data-grid';
import { Button } from '@mui/material';
import { fileData } from '../main';

const columns: GridColDef[] = [
  {
    field: 'id',
    headerName: 'ID',
    description: 'This column has a value getter and is not sortable.',
    sortable: false,
    width: 50,
    valueGetter: (params: GridValueGetterParams) =>
      params.row.id,
  },
  {
    field: 'dir',
    headerName: 'Dir',
    type: 'string',
    width: 70,
    editable: true,
  },
  {
    field: 'doc',
    headerName: 'Doc',
    type: 'string',
    width: 200,
    editable: true,
  },
  {
    field: 'artboard',
    headerName: 'Artboard',
    type: 'string',
    width: 150,
    editable: true,
  },
  {
    field: 'output',
    headerName: 'Output',
    type: 'string',
    width: 200,
    editable: true,
  },
  {
    field: 'w',
    headerName: 'Width',
    type: 'string',
    width: 70,
    editable: true,
  },
  {
    field: 'h',
    headerName: 'Height',
    type: 'string',
    width: 70,
    editable: true,
  },

];

let startRows = [
  {
    id: 0,
    dir: 'home',
    doc: 'background-effect.ai',
    artboard: 'Artboard 1',
    output: 'background-effect',
    w: '100',
    // size: { w: 100 },
  },
  {
    id: 1,
    dir: 'home',
    doc: 'background-effect.ai',
    artboard: 'Artboard 1',
    output: 'background-effect',
    w: '100',
    // size: { w: 100 },
  },
  {
    id: 2,
    dir: 'home',
    doc: 'background-effect.ai',
    artboard: 'Artboard 1',
    output: 'background-effect',
    w: '100',
    // size: { w: 100 },
  },
  {
    id: 3,
    dir: 'home',
    doc: 'background-effect.ai',
    artboard: 'Artboard 1',
    output: 'background-effect',
    w: '100',
    // size: { w: 100 },
  },
];


export default function MyTable() {
  const [rows, setRows] = React.useState(startRows) 
  const [selectedRow, setSelectedRow] = React.useState <any>(null) 

  function addRow(){
    let emptyObj =     
        {
            id:rows.length,
            dir: '',
            doc: '',
            artboard: 'Artboard 1',
            output: '',
            w: '',
            h: ''
        }
        setRows([...rows, emptyObj])
        // rows.push(emptyObj) //chi ashxatum
  }

  function delRow(){
    selectedRow && setRows( rows.filter((el)=>{ return el.id !== selectedRow.id}))
  }


  React.useEffect(()=>{
    // console.log('refresh', rows)
    // console.log(fileData)
  },[rows])

  return (
    <Box sx={{  width: '100%' }}>
      <Button variant='contained' onClick={()=>{addRow()}}>Add Row</Button>
      <Button variant='contained' onClick={()=>{delRow()}}>Delete Row</Button>

      <DataGrid
        rows={rows}
        columns={columns}
        onRowClick={(e:any)=>{setSelectedRow(e.row)}}
        columnVisibilityModel={{ id: false,}} //chereva Idn
        initialState={{
          pagination: {
            paginationModel: {
              pageSize: 10,
            },
          },
        }}
        pageSizeOptions={[10]}
        showCellVerticalBorder={true}
        // disableRowSelectionOnClick = {true} //vor select chlini
      />
    </Box>
  );
}
