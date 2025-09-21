import {getBoolSetting, getParsedSetting, getSortedFiles,  getStringSetting} from "getSettings"
import parseBool from "parse"

export class StateContext {

    // wireFrame = false ;
    // vertices = false ;
    // faceCulling = false;
    // depthTest = true ;


    
   
    wireFrame = getBoolSetting('wireFrame', false)
    vertices = getBoolSetting('vertices', false)
    faceCulling = getBoolSetting('faceCulling', false)
    depthTest = getBoolSetting('depthTest', true)
    lighing = getBoolSetting('lighting', true)
    scale = getStringSetting('scale', '1')
    shininess = getStringSetting('shininess', '5')
    lightX =  getStringSetting('lightX', '')
    lightY =  getStringSetting('lightY', '')
    lightZ =  getStringSetting('lightZ', '')
    ambientColor = getStringSetting('ambientColor', '#d61c41')
    diffuseColor = getStringSetting('diffuseColor', '#ffff')
    specularColor = getStringSetting('specularColor', '#ffff')

    fogType =  getStringSetting('fogType', 'None')
    fogDepth = getStringSetting('fogDepth', 'distance')
    fogColor = getStringSetting('fogColor', '#ffff')
    fogDensity  = getStringSetting('fogDensity', '')
    zNear  = getStringSetting('zNear', '')
    zFar  = getStringSetting('zFar', '')

    models = getParsedSetting('models', []);
    animations = getParsedSetting('animations', []);
    backgrounds = getParsedSetting('backgrounds', []);
    blueprints = getParsedSetting('blueprints', []);
    currentFile = getParsedSetting('currentFile', {});

    currentTab = 'backgrounds'



    loadModelProperties() {
        Module.call("setModelWireframe", this.wireFrame.toString());
        Module.call("setModelVertices", this.vertices.toString());
        Module.call("setModelFaceCulling", this.faceCulling.toString());
        Module.call("setModelDepthTest", this.depthTest.toString());
        Module.call("setModelLighting", this.lighing.toString());
        Module.call("setModelScale", this.scale);
        Module.call("setModelLightingPosition", JSON.stringify([this.lightX, this.lightY, this.lightZ]));
        Module.call("setModelMaterial", JSON.stringify([this.ambientColor, this.diffuseColor, this.specularColor, this.shininess]));
        Module.call("setModelFog", JSON.stringify([this.fogType, this.fogDepth, this.fogColor, this.fogDensity, this.zNear, this.zFar]));

        
    }



    // constructor() {
    // //   Module.call("setModelWireframe", this.wireFrame.toString());
    // //   Module.call("setModelVertices", this.vertices.toString());
    // //   Module.call("setModelFaceCulling", this.faceCulling.toString());
    // //   Module.call("setModelDepthTest", this.depthTest.toString());
    // }

    // getBoolSetting(name: string, defaultOption: boolean) {
    //     let item = localStorage.getItem(name)
    //     if (item) {
    //         return parseBool(item)
    //     } else {
    //         return defaultOption
    //     }

    // }





    canvasWidth = 320;
    canvasHeight = 480;



};

