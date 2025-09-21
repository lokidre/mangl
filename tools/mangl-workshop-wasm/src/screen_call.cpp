/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "screen.h"

#include <apeal/hash.hpp>   
#include <apeal/string.hpp>
#include <apeal/parse/json.hpp>
#include <apeal/file.hpp>




#include "screen_model.h"
#include "../src/core/mangl_parse.h"
#include "../include/apeal/str/case.hpp"



#include <iostream>



String Screen::onCall(StrView name, StrView data) {
    using namespace apl;



    // struct pos = {10.1f, 5.3f, 8.6f};
    // const float* pData= new const float[3]{1.1,2.2,3.3};
    // for(string cor: data) {
    //     pos.push_back(std::stoi(cor));
    // }



    //std::cout << "Screen:onCall: " << name << ": " << data << std::endl; 

    switch(pshHash(name)) {
    case "setModelWireframe"_psh:
        model_.wireframe = parseBool(data);
        //model_.wireframe = true;
        break;
    case "setModelVertices"_psh:
        model_.vertices = parseBool(data);
        break;
    case "setModelFaceCulling"_psh:
        model_.faceCulling = parseBool(data);
        break;
    case "setModelDepthTest"_psh:
        model_.depthTest = parseBool(data);
        break;
    case "setModelLighting"_psh:
        model_.lightingEnabled = parseBool(data);
        break;
    case "setModelScale"_psh: {
        auto scale = parseReal<RealGL>(data);

        // //model_.scale(scale);
        modelScale_ = modelSphereSize_ / maxFace_;
        modelScale_ *= scale;
        modelMatrix_.scaling(modelScale_);
        modelMatrix_.translate(-modelCentroid_);
        currentAngle_.rotateMatrix(modelMatrix_);


    //std::cout << modelScale_ << std::endl;
        // model_.scale(scale);
        break;
    }
    case "setModelLightingPosition"_psh: {


        auto json = apl::parseJson(data);
        //file.save("test.json", json);
         lightPosition_ = {parseFloat(json.nodes[0].value), parseFloat(json.nodes[1].value), parseFloat(json.nodes[2].value)};


        // lightingPosition = apl::parseJson(data);
        model_.lightPosition =  &lightPosition_;
        // std::cout << parseInt( json.nodes[0].value) << std::endl;
        //std::cout << json.asString() << std::endl;
        break;
        }
    case "setModelMaterial"_psh:{
        auto json = apl::parseJson(data);


        auto ambientColor = parseColor(json.nodes[0].value);
        auto diffuseColor = parseColor(json.nodes[1].value);
        auto specularColor = parseColor(json.nodes[2].value);
        auto shininess = parseReal<RealGL>(json.nodes[3].value);


        model_.setMaterial(ambientColor, diffuseColor, specularColor, shininess);



        std::cout << json.nodes[0].value << std::endl;
        std::cout << "Hello" << std::endl;

        break;
    }
    case "setModelFog"_psh:{
        auto json = apl::parseJson(data);



        auto fogType = str_lower(json.nodes[0].value) ;
        auto depthType = str_lower(json.nodes[1].value);
        auto fogColor = parseColor(json.nodes[2].value);
        auto fogDensity = parseReal<RealGL>(json.nodes[3].value);
        auto fogZnear =  parseReal<RealGL>(json.nodes[4].value);
        auto fogZfar =  parseReal<RealGL>(json.nodes[5].value);

        if(fogType == "linear") {
            fog_.type = Fog::Type::Linear;
//            std::cout << "at linear" << std::endl;
        } else if (fogType == "exponential") {
            fog_.type = Fog::Type::Exp;
//            std::cout << "at exp" << std::endl;
        } else if (fogType == "expsquare") {
            fog_.type = Fog::Type::ExpSquare;
//            std::cout << "at exps" << std::endl;
        } else if(fogType == "none") {
            model_.fog = nullptr;
//            std::cout << "at none" << std::endl;
            break;
        }

        if(depthType == "distance"){
            fog_.depth = Fog::Depth::Distance;
//            std::cout << "at dist" << std::endl;
        } else if (depthType == "range") {
            fog_.depth = Fog::Depth::Range;
//            std::cout << "at range" << std::endl;
        }



        fog_.set(fogColor, fogDensity, fogZnear, fogZfar);
        model_.fog = &fog_;


        break;
    }
    case "deleteModel"_psh: {
        buildEmptyBackground();
        break;
    }
    case "setModel"_psh: {
        auto model = data;
        modelData_ = model;
//        model_.loadObjData(modelData_);
        buildModel();
        std::cout << "model Loaded" << std::endl;
        break;

    }
    case "setAnimation"_psh: {
        auto animation = data;
        animationData_ = animation;
        buildAnimation();
        std::cout << "starting animation" << std::endl;
//
        break;

    }
    case "setBackground"_psh: {

        std::cout << "setting background" << std::endl;
        break;

    }
    case "setBlueprint"_psh: {

        std::cout << "setting blueprint" << std::endl;
        break;

    }





    default:
        throw std::runtime_error(fmt("Unknown call name: '%s'", name));
        //return fmt(R"({"error": "Unknown call name: '%s'"})", name);
        //break;
    }

    return "{\"ok\": 1}";
}

