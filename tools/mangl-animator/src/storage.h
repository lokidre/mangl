/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

#include <mangl/environment.h>
#include <mangl/storage.h>

#include <apeal/singleton.hpp>

namespace animator {

struct Settings: public apl::Singleton<Settings> {

    //Setting<String> zoomTag{"zoom_tag", "100"};

    //
    // Main window state
    //
    Setting<String> mainWindowState{"main_window_state"};
    Setting<String> mainWindowGeometry{"main_window_geometry"};
    Setting<String> mainSplitterState{"main_splitter_state"};

    Setting<int> currentTabIndex{"current_tab_index"};
    Setting<String> lastOpenFileDialogState{"last_open_file_dialog_state"};

    //
    // Animator state
    //
    Setting<String> currentFile{"current_file"};
    Setting<String> workingDirectory{"working_directory"};
    Setting<int> selectedAnimation{"selected_animation", -1};

    //
    // Layout and screen size
    //
    Setting<int> zoom{"zoom_percent", 100};

    Setting<String> lastOpenDir{"last_open_dir"};
    Setting<String> screenSizeTag{"screen_size_tag"};

    Setting<bool> customLayoutOn{"custom_layout_on", false};
    Setting<SizeI> customLayoutSize{"custom_layout_size"};

    Setting<bool> orientationLandscape{"orientation_landscape", false};

    //
    // Tools
    //
    Setting<String> editorBinPath{"editor_bin_path"};
};

}  // namespace animator
