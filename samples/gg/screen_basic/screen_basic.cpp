/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../common/framework.h"

#include <mangl/texture.h>
#include <mangl/widgets.h>
#include <mangl/blueprint.h>

class AppScreen: public ScreenBase {
protected:
    void onStart() override;

    mangl::blueprint::Loader obj;
};

//TextureAtlas gameTex;

using namespace mangl::blueprint;

//Scene scene;
//Blueprint blueprint;

void AppScreen::onStart()
{   
    obj.loadFunction("../data/objects.yaml");
    
    //obj.load

    //Engine eng;
    //auto eng = new Engine(); //TODO

    //auto scene = Framework::instance()->scene();
    //obj.load("../data/project.yaml", &eng->blueprint_);

    //eng->create(scene);



    //auto project =loadProject("../data/project.yaml");///Users/Arus/dev/mangle/samples/gg/screen_basic/data/rectangle.yaml
    
    //gameTex.load("C:\\Users\\Arus\\Dropbox\\apps\\pentix\\textures\\320x480\\tex_gui");
    //auto& font = gameTex.getFont("gui_font_medium");

    //auto fontNames = loadFontNames(project);
    
    //createRect(project);
    //Index screenIndex = 0;

   //createScreenComponents(project, screenIndex,gameTex);
   
    
    //Cord textHeight{};
    //Color labelColor{M::Silver};
    //Color textColor{M::White};
  

    //auto debugRect = [&](RectA r) {
    //    auto c = M::White;// c{0xFFFFFF} ;
    //    auto w = 1.;
    //    scene->newPoint(r.position(), c, w);
    //    scene->newPoint(r.position().offset({0, r.h}), c, 1);
    //    scene->newPoint(r.extent(), c, w);
    //    scene->newPoint(r.position().offset({r.w, 0}), c, w);
    //};

    //auto testRect = [&](RectA r, ColorA color, Cord width = 0) {
    //    auto prim = scene->newRect(color, r);

    //    if (width)
    //        prim->setWidth(width);

    //    debugRect(r);
    //    return prim;
    //};

    //testRect(calcCoords(project), 0xFF0000, 4);

    //scene->newLabel("Start", {160, 50}, textHeight, textColor, M::AlignCenterAll)->setFont(&font);
    //scene->newLabel(getText(project), {160, 50}, textHeight, textColor, M::AlignCenterAll)->setFont(&font);
    //createLabel(project);
    

    
}

#include "../../common/framework.cpp"
