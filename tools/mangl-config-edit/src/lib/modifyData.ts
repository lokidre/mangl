import * as YAML from 'yaml';
import hardCodedFile from '../setup/mangl_config_setup.yaml';
import {YamlPackage, YamlProp, YamlSection} from './yamlPackageClasses';
// import hardCodedFile from '../public/hardCodedVars.yaml'

//const crypto = require('crypto');

const key = '1234567890123456'; // 16 bytes for AES-128 
const iv = '1234567890123456';  // 16 bytes for IV 

export function genEncrFileContent(plaintext:any){
    return plaintext;
    // const encrypted = encryptAES128CBC(key, iv, JSON.stringify(plaintext)); 
    // return encrypted
}

 
// function encryptAES128CBC(key:any, iv:any, plaintext:any) { 
//     const cipher = crypto.createCipheriv('aes-128-cbc', Buffer.from(key), Buffer.from(iv)); 
//     let encrypted = cipher.update(plaintext, 'utf8', 'hex'); 
//     encrypted += cipher.final('hex'); // PKCS7 padding is applied here 
//     return encrypted; 
// } 
 


export function getAddedProps(){
    return {
        id: new Date().getTime() + Math.floor(Math.random()*1000000000), //todo
        editable: true 
    }
}


export class VarObj {
    id!: number;
    name!: string;
    value!: string;
    description!: string;
    editable?: boolean;
    selection?: string[]
    type?:  'string' | 'int' | 'bool' | 'real' | 'selection'
    required?:boolean
}





export function JSONtoYAML(jsonStr: string) {
    let obj = JSON.parse(jsonStr);
    let yamlStr = YAML.stringify(obj);
    return yamlStr;
}

export function YAMLtoJSON(yamlStr: string) {
    let obj = YAML.parse(yamlStr);
    let jsonStr = JSON.stringify(obj);
    return jsonStr;
}

export function serverYmlDataToClient(data: any) {
    // const parsedData = JSON.parse(YAMLtoJSON(data));
    let modData = modUplData(data);
    console.log("modData", modData)
    // return jsonToPackage(modData);
    return modData
}

function createNewVarObj(dataEl:any, editable:boolean):VarObj{
    let addedProps = JSON.parse(JSON.stringify(getAddedProps()))
    addedProps.editable = editable
    return  {...dataEl, ...addedProps }
}

function mergeArrs(hardP:any[], myDataP:any[]){
    let hardRes:any[] = []
    let other:any[] = []

    let hardPCopy:any[] = JSON.parse(JSON.stringify(hardP))
    let myDataPCopy:any[] = JSON.parse(JSON.stringify(myDataP))


    for(let j = 0; j < hardPCopy.length; j++ ){
        for(let i = 0; i < myDataPCopy.length; i++ ){

            if(hardPCopy[j].name === myDataPCopy[i].name){
                hardPCopy.splice(j, 1);
                --j
                let remEl = myDataPCopy.splice(i, 1);
                // console.log(...remEl)
                hardRes.push(...remEl)
                --i                
                break
            }
        }
    }
    if(hardPCopy.length) hardRes = [...hardRes, ...hardPCopy]
    if(myDataPCopy.length) other = myDataPCopy

    return {hardProps: hardRes, otherProps: other}
}



export function modUplData(data: any) { //YamlPackage
    const hardDataFileCont = hardCodedFile as YamlPackage; //yaml a, bayc json a kardum
    const hardData = hardDataFileCont.config.sections;

    let tmpHardCode: YamlSection[] = JSON.parse(JSON.stringify(hardData));
    let tmpData: YamlSection[] = [];
    let otherS = new YamlSection('OTHER');
    
    let myDataSecs = data.config.sections
    let otherExist = false


     myDataSecs.forEach((mySec:any)=>{
        let mySecExsistObj = tmpHardCode.find((sec:any) => { return sec.name === mySec.name}) 
        if (!mySecExsistObj){ //ete chka im data i mej
            if(mySec.name === "OTHER"){
                otherS = mySec
                otherExist = true
            }
            tmpData.push(mySec)
        }
    })


    tmpHardCode.forEach((hardSec:any)=>{
        let mySecExsistObj = myDataSecs.find((sec:any) => { return sec.name === hardSec.name}) 
        if (mySecExsistObj){ //ete ka im data i mej
          let mergedArrs = mergeArrs(hardSec.properties, mySecExsistObj.properties)
          hardSec.properties = mergedArrs.hardProps
          otherS.properties.push(...mergedArrs.otherProps)
        }
    })  


    //add properties
    tmpHardCode.forEach((hardSec:any)=>{
        hardSec.properties = hardSec.properties.map((hardEl:any) => {
            let addedProps  = getAddedProps()
            hardEl = {...hardEl, ...addedProps}
            hardEl.editable = false
            return hardEl
        });
    })  

    otherS.custom = true
    otherS.properties = otherS.properties.map((el:any)=>{
        let addedProps  = getAddedProps()
        return {...el, ...addedProps}
    })  

    tmpData.forEach((sec:any)=>{
        sec.custom = true
        sec.properties = sec.properties.map((el:any) => {
            let addedProps  = getAddedProps()
            return {...el, ...addedProps}
        });
    })  

    if(otherS.properties.length && !otherExist)  return {config: {sections:[...tmpHardCode, otherS,  ...tmpData]}}
    
    else  return {config: {sections:[...tmpHardCode, ...tmpData]}}

}

function jsonToPackage(data: YamlSection[]) {//der chem ogtagorcum
    console.log('jsoooooooooooooooooooooooooooooooooooooooooooon', data)
    let arr: any[] = [];
    data.map((obj: any, i: number) => {
        let tmpObj: VarObj = {
            id: new Date().getTime() + i,
            name: obj.name,
            value: obj.value,
            description: obj.descr,
            editable: obj.editable ? true : false,
        };
        arr.push(tmpObj);
    });
    return arr;
}

export function modExportFile(uplData: VarObj[]) {     
    let res:any = {
        config: {
            sections: []
        }
    };

    uplData.forEach((dataSection: YamlSection) => {
        let newDataS = new YamlSection(dataSection.name, dataSection.description,)
        dataSection.properties?.map((el:any)=>{
            let obj = JSON.parse(JSON.stringify(el))
            delete obj.id //todo aveli sirun/harmar
            delete obj.editable
            newDataS.properties.push(obj)
        })   
        res.config.sections.push(newDataS)
    })
    console.log(res)
    return res
}


export function downloadFile(jsonArr: any, fileName: string, ext:string) {
    let data = JSON.stringify(jsonArr)

    if(ext === 'yaml'){
        data = JSONtoYAML(data);
    }
    fileName = fileName.replace(/\.\w+$/, '.' + ext)
    
    const blob = new Blob([data], {type: 'text/plain'});
    const url = window.URL.createObjectURL(blob);
    const a = window.document.createElement('a');
    a.href = url;
    a.download = fileName;
    a.click();
    window.URL.revokeObjectURL(url);
}
