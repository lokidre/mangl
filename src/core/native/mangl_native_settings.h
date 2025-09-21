/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

namespace mangl {


struct MNativeSettings {
    MNativeSettings();

    static void setValue(MStringA key, MStringA value, MStringA suffix = MString());
    static void setValue(MStringA key, int value, MStringA suffix = MString());
    static void setValue(MStringA key, bool value, MStringA suffix = MString());
    static void setValue(MStringA key, MFloat value, MStringA suffix = MString());

    static void set(MStringA key, MStringA value, MStringA suffix = MString()) { setValue(key, value, suffix); }
    static void set(MStringA key, int value, MStringA suffix = MString()) { setValue(key, value, suffix); }
    static void set(MStringA key, bool value, MStringA suffix = MString()) { setValue(key, value, suffix); }
    static void set(MStringA key, MFloat value, MStringA suffix = MString()) { setValue(key, value, suffix); }


    static void getValue(MStringA key, MString &value, MStringA def = MString(), MStringA suffix = MString());
    static void getValue(MStringA key, int &value, int def = 0, MStringA suffix = MString());
    static void getValue(MStringA key, bool &value, bool def = false, MStringA suffix = MString());
    static void getValue(MStringA key, MFloat &value, MFloat def = 0, MStringA suffix = MString());

    static MString stringValue(MStringA key, MStringA def = MString(), MStringA suffix = MString()) {
        MString val;
        getValue(key, val, def, suffix);
        return val;
    }

    static int intValue(MStringA key, int def = 0, MStringA suffix = MString()) {
        int val;
        getValue(key, val, def, suffix);
        return val;
    }

    static bool boolValue(MStringA key, bool def = false, MStringA suffix = MString()) {
        bool val;
        getValue(key, val, def, suffix);
        return val;
    }

    static MFloat floatValue(MStringA key, MFloat def = 0, MStringA suffix = MString()) {
        MFloat val;
        getValue(key, val, def, suffix);
        return val;
    }

};

}  // namespace mangl
