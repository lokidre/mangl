import React from 'react'
import {GlobalContext} from './GlobalContext'

export type GlobalStateValue = {
    state: GlobalContext
    setState: React.Dispatch<React.SetStateAction<GlobalContext>>
}

export const GlobalStateDefValue: GlobalStateValue  = {
    state: new GlobalContext(),
    setState: (state) => {},
}

export const GlobalStateInstance = React.createContext(GlobalStateDefValue)
