/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../game/mangl_game_screen.h"
#include "../mangl_texture_atlas.h"
#include "../mangl_framework.h"
#include "../game/mangl_leaderboard.h"
#include "../widgets/mangl_table_widget.h"



namespace mangl {

struct GameHighScoresScreenSetup {
    StrView titleText;
    Texture titleTexture;
    bool closeButton;

    LeaderboardP leaderboard;
    std::initializer_list<LeaderboardP> leaderboards;
};



class HighScoresScreenTableModel: public TableWidgetModel {
    LeaderboardP leaderboard_{};

public:
    void setLeaderboard(LeaderboardP leaderboard) noexcept { 
        leaderboard_ = leaderboard;
    }

protected:
    int columnCount() override { return 2; }
    int rowCount() override { return 25; }

    void onCreateCell(TableWidgetCellInfo& ci) override {
        const auto& ss = M::Styles::table;

        ci.height = ss.rowHeight;
        ci.padding = ss.cellPadding; //Padding{4};

        Flags alignment = (ci.col == 1 ? M::AlignRight : M::AlignLeft);
        alignment |= M::AlignVCenter;

        ci.widget = Framework::getScene()->newLabel({
            .style = &ss.contentText,
            .flags = alignment,
        });
    }

    void onCreateSeparator(TableSeparatorInfo& sep) override {
        const auto& ss = M::Styles::table;

        SeparatorStyles sepStyle{};

        switch (sep.type) {
        using enum TableSeparatorInfo::Type;
        case HorzBorder:
            sepStyle = ss.horzBorder;
            break;
        case VertBorder:
            sepStyle = ss.vertBorder;
            break;
        case HorzSep:
            sepStyle = ss.horzSep;
            break;
        case VertSep:
            sepStyle = ss.vertSep;
            break;
        }

        if (sepStyle.width == 0)
            return;

        sep.width = sepStyle.width;
        sep.prim = Framework::getScene()->newBox(sepStyle.color);
    }

    void onUpdateData(TableWidgetCellInfo& ci) override {
        auto label = dynamic_cast<LabelP>(ci.widget);
        ADebugTrapAssert(label);

        if (ci.col == 0)
            label->setText("%d.", ci.row + 1);

        if (ci.col == 1)
            (ci.row < leaderboard_->count()) ? label->setInt(leaderboard_->getValue(ci.row)) : label->clearText();
    }
};


class HighScoresScreenContentsWidget {
protected:
    GameScreenBaseP screen_;
    //GameHighScoresScreenSetup params_{};

    TableWidgetP table_{};
    std::unique_ptr<HighScoresScreenTableModel> dataModel_;

    std::vector<LeaderboardP> leaderboards_;


public:
    HighScoresScreenContentsWidget(GameScreenBaseP screen): screen_{screen} {}

    void setup(const GameHighScoresScreenSetup& params);

    void setup(LeaderboardP leaderboard) {
        setup({
            .leaderboard = leaderboard,
        });
    }

    void setup(Texture titleTexture, LeaderboardP leaderboard) {
        setup({
            .titleTexture = titleTexture,
            .leaderboard = leaderboard,
        });
    }


    void onStart();
};



template <class BaseScreenT>
class HighScoresScreenBase: public BaseScreenT {

public:
    void setup(const GameHighScoresScreenSetup& setup) {
        widget_.setup(setup);
    }

    void setup(LeaderboardP leaderboard) {
        setup({
            .leaderboard = leaderboard,
        });
    }

    void setup(Texture titleTexture, LeaderboardP leaderboard) {
        setup({
            .titleTexture = titleTexture,
            .leaderboard = leaderboard,
        });
    }



private:
    HighScoresScreenContentsWidget widget_{this};

protected:
    void onStart() override { widget_.onStart(); }
};


}  // namespace mangl


