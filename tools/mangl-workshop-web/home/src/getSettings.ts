import parseBool from "parse"

export function getBoolSetting(name: string, defaultOption: boolean) {
    let item = localStorage.getItem(name)
    if (item) {
        return parseBool(item)
    } else {
        return defaultOption
    }

}

export function getStringSetting(name: string, defaultOption: string) {
    let item = localStorage.getItem(name)
    if (item) {
        return item
    } else {
        return defaultOption
    }


}


export function getParsedSetting(name: string, defaultOption: any) {
    let item = JSON.parse(localStorage.getItem(name))
    if (item) {
        return item
    } else {
        return defaultOption
    }


}


export function getSortedFiles(tabIndex: number, defaultOption: string) {

    let files = JSON.parse(localStorage.getItem('files'))
    let sortedFiles = []
    if(files) {
        for(let file of files) {
            if(file.tab === tabIndex) [
                sortedFiles.push(file)
            ]
        }
        return sortedFiles
    }
    
    return defaultOption
    
    
   
}

// export function getCurrentFile(defaultOption: string) {

//     let files = JSON.parse(localStorage.getItem('files'))
//     if(files) {
//         for(let file of files) {
//             if(file.current === true) {
//                 return file
//             }
//         }
//     }
    
//     return defaultOption
    
    
   
// }

