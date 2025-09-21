import { Project } from "../setup/project";

export class ProjectStateContext {
    loaded = false;
    unsaved = false;

    project?: Project;
}

