import React from 'react';

import Home from './Home';
import { ProjectStateDefValue, ProjectStateInstance } from './ProjectStateProvider';

export default function MainWindow() {
    const [projectCtx, setProjectCtx] = React.useState(ProjectStateDefValue.state);

    return (
        <ProjectStateInstance.Provider value={{state: projectCtx, setState: setProjectCtx}}>
            <Home/>
        </ProjectStateInstance.Provider>
    );
}
