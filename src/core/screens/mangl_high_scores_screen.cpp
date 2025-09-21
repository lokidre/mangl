/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_high_scores_screen.h"

#include "../widgets/mangl_table_widget.h"
#include "../game/mangl_leaderboard.h"

namespace mangl {

void HighScoresScreenContentsWidget::setup(const GameHighScoresScreenSetup& params) {
    //params_ = params;

    if (params.leaderboard) 
        leaderboards_.push_back(params.leaderboard);

    for (auto lb: params.leaderboards) {
        leaderboards_.push_back(lb);
    }

    StrView titleText = params.titleText;
    if (titleText.empty() && !params.titleTexture)
        titleText = "HIGH SCORES";

    screen_->setup({
        .type = GameScreenType::HighScores,
        .titleText = titleText,
        .titleTexture = params.titleTexture,
        .closeButton = true, //params_.closeButton,
    });
}



void HighScoresScreenContentsWidget::onStart() {
    auto scene = screen_->mainScene_;
    auto safe = screen_->contentArea_;

    const auto& stable = M::Styles::table;

//#if MANGL_GAMECENTER
//    GameCenter::authenticate();
//
//    if (Env::gamecenterEnabled) {
//        auto rect = Gui::layout.closeButtonRect;
//        rect.x = full.x + full.right() - rect.right();
//        scene->newButton({
//            .rect = rect,
//            .margins = Gui::layout.closeButtonMargins,
//            .texture = Gui::guiTex.highScoresGamecenterButton,
//            .onPush = [this] {
//                GameCenter::launchLeaderboardGui(leaderboards_.marathon.gameCenterId);
//            }
//        });
//    }
//#endif

    Cord paddingTop = safe.h - screen_->screenTitle_.rect.y;
    paddingTop += 10;
    Cord paddingLeft = safe.x == 0 ? 30 : safe.x;
    Cord paddingBottom = safe.y + 10; // safe.y == 0 ? 20 : safe.y;

    // High Scores table
    Rect tableRect{paddingLeft, paddingBottom, safe.w - paddingLeft*2, safe.h - paddingTop - paddingBottom};
    //scene->newBox(Gui::theme.screenPanelBackgroundColor, tableRect);
    scene->newBox(stable.backgroundColor, tableRect);

    dataModel_ = std::make_unique<HighScoresScreenTableModel>();

    MDebugTrapAssert(!leaderboards_.empty());

    dataModel_->setLeaderboard(leaderboards_[0]);

    table_ = scene->newWidget<TableWidget>()->move(tableRect)->setModel(dataModel_.get())->create();

    table_->updateData();

}

}  // namespace mangl
