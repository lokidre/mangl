import React from 'react';
import { ProjectStateContext } from './ProjectStateContext';

export type ProjectStateValue = {
    state: ProjectStateContext;
    setState: React.Dispatch<React.SetStateAction<ProjectStateContext>>;
};

export const ProjectStateDefValue: ProjectStateValue = {
    state: new ProjectStateContext(),
    setState: (state: ProjectStateContext) => {},
};

export const ProjectStateInstance = React.createContext(ProjectStateDefValue);
