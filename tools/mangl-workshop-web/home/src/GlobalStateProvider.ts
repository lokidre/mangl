import React from 'react'
import {StateContext} from './data'

export type GlobalStateValue = {
    state: StateContext
    setState: React.Dispatch<React.SetStateAction<StateContext>>
}

export const GlobalStateDefValue: GlobalStateValue  = {
    state: new StateContext(),
    setState: (state) => {},
}

export const GlobalStateInstance = React.createContext(GlobalStateDefValue)
