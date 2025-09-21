import { Container, TextField, Button, List, ListItem, Checkbox, Typography, Box, Stack, styled, Paper,} from "@mui/material";
import React from "react";
import { useState } from "react";
import DeletePopup from "./DeletePopup";
import { VarObj } from "../lib/modifyData";
import SelectVals from "./SelectVals";



export function getType(inputType:string){
    switch (inputType){
        case 'string':{
            return 'string'
        }
        case 'int':{
            return 'number'
        }
        case 'bool':{
            return 'bool'
        }
        default:{
            return 'string'
        }            
    } 
}


interface ListElementProps{
    obj: VarObj
    saveElEdit: (obj:VarObj)=>void
    delEl: (obj:VarObj)=>void
}


export default function ListElement(props:ListElementProps){
    const [inputValue, setInputValue] = useState(props.obj.value || '');
    const [inputName, setInputName] = useState(props.obj.name|| '');
    const [inputDescr, setInputDescr] = useState(props.obj.description|| '');
    const [req, setReq] = useState(props.obj.required);
    
    const onChangeVal = (e: any) => {
        setInputValue(e.target.value);
    };

    const onChangeName = (e: any) => {
        setInputName(e.target.value);
    };

    const onChangeSelect = (val: any) => {
        setInputValue(val);
    };


    const onDelete = () => {
        props.delEl(props.obj)
    };


    React.useEffect(()=>{
        props.obj.value = inputValue
        props.obj.name = inputName
        props.obj.description = inputDescr
        if(props.obj.required === true){
            !(inputValue) ? setReq(true) : setReq(false)
        }
    },[inputValue, inputName, inputDescr])

    
    return <>
        <TextField label="name" disabled={!props.obj.editable}  variant="outlined" onChange={onChangeName} value={inputName}/> 

        {props.obj.type === "selection" ? 
            <SelectVals required={req} selections={props.obj.selection} onChange={onChangeSelect} value={inputValue}/>
        :
            <TextField error={req} label="value" type={getType(inputValue)}  onChange={onChangeVal} value={inputValue}/>
        }

        <Typography sx={{padding: "10px"}}> 
            {props.obj.type} 
            <span style={{fontSize: "10px", display:'block'}}> {inputDescr} </span>

        </Typography>

        {props.obj.editable &&  <DeletePopup onDelete={ onDelete} />}
    </>
}