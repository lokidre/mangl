/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_button.h"
#include "mangl_label.h"
#include "mangl_background_widget.h"
#include "mangl_stack_widget.h"
#include "mangl_dialog_params.h"
#include "mangl_widget_border.h"

#include <apeal/signal.hpp>

namespace mangl {

struct Dialog: public WidgetTemplate<Dialog> {
    friend class Scene;

    using Self = Dialog;
    using Base = WidgetTemplate<Self>;

    using SelfP = Self*;
    using SelfRet = Self*;

    Dialog() noexcept = default;

    DialogP clear();
    SelfRet clearAll();

    SelfRet move(RectA r) {
        userLayout_ = r;
        r_ = r;
        updateLayout();
        return this;
    }
    //[[nodiscard]] const auto& getRect() const noexcept { return r_; }

    void run();
    void runModless();
    void show(bool nonmodal = false);

    void close();

    [[nodiscard]] bool active() const noexcept { return active_; }


    [[nodiscard]] auto getTheme() const noexcept { 
        return params_.theme ? params_.theme : &M::Styles::dialog;
    }

    auto backgroundWidget() { return backgroundWidget_; }
    auto titleLabel() { return titleLabel_; }
    auto messageLabel() { return messageLabel_; }

    auto confirmButton() { return confirmButton_; }
    auto rejectButton() { return rejectButton_; }

private:

    // Creation params
    DialogParams params_{};
    bool haveConfirmButton_{};
    bool haveRejectButton_{};


    // Layout
    Rect userLayout_{};

    // Contents
    struct Item {
        PrimP prim{};
        WidgetP widget{};
        bool active{true};

        void setVisible(bool flag);
    };

    std::vector<Item> permanentItems_, items_;
    std::vector<WidgetP> interactiveWidgets_;


    // Dialog GUI elements
    PrimP backgroundPrim_{};
    //StackWidgetP backgroundWidget_{};
    WidgetP backgroundWidget_{};
    StackWidgetP backgroundStackWidget_{};

    WidgetBorderStrikesRuntime borderRuntime_{};



    // Title
    String titleText_;
    LabelP titleLabel_{};


    // Contents message
    String messageText_;
    LabelP messageLabel_{};


    //
    // Dialog buttons
    //
    String confirmButtonText_{};
    std::function<void()> confirmSignal_{};
    ButtonP confirmButton_{};

    String rejectButtonText_{};
    std::function<void()> rejectSignal_{};
    ButtonP rejectButton_{};


    // Dialog orientation
    M::Orientation orientation_{M::Orientation::Orientation0};


    // Dialog status
    bool active_{};

    void updateLayout();


private: // runtime tracking
    bool closeOnAction_{};


public:
    auto setType(DialogType t) {
        params_.type = t;
        return this;
    }


    auto setTitle(StrView title) {
        titleText_ = title;
        if (titleLabel_) titleLabel_->setText(title);
        return this;
    }


    auto setMessage(StrView text) {
        messageText_ = text;
        if (messageLabel_) messageLabel_->setText(text);
        return this;
    }


    auto setConfirmButtonText(StrView text) {
        confirmButtonText_ = text;
        if (confirmButton_) confirmButton_->setText(text);
        return this;
    }

    void setConfirmSignal(SlotVoid signal);
    void setRejectSignal(SlotVoid signal);


    auto setRejectButtonText(StrView text) {
        rejectButtonText_ = text;
        if (rejectButton_) rejectButton_->setText(text);
        return this;
    }


    auto addWidget(PrimP prim) {
        items_.push_back({.prim = prim});
        return this;
    }

    auto addWidget(WidgetP widget) {
        items_.push_back({.widget = widget});
        return this;
    }

    auto addPermanentWidget(PrimP prim) {
        Item item;
        item.prim = prim;
        permanentItems_.emplace_back(item);
        return this;
    }

    auto addPermanentWidget(WidgetP widget) {
        Item item;
        item.widget = widget;
        permanentItems_.push_back(item);
        return this;
    }

    template <class ContainerT>
    auto addWidgets(const ContainerT& prims) {
        for (auto& prim: prims)
            addWidget(prim);
        return this;
    }

    auto addWidgetsN(PrimP prims, int total) {
        for (auto i = 0; i < total; ++i)
            addWidget(prims + i);
        return this;
    }



    auto setConfirmButton(ButtonP confirmButton) {
        confirmButton_ = confirmButton;
        return addPermanentWidget(confirmButton);
    }

    auto setRejectButton(ButtonP rejectButton) {
        rejectButton_ = rejectButton;
        return addPermanentWidget(rejectButton);
    }

    auto setTitleLabel(LabelP widget) {
        titleLabel_ = widget;
        return addPermanentWidget(widget);
    }

    auto setMessageLabel(LabelP widget) {
        messageLabel_ = widget;
        return addPermanentWidget(widget);
    }


    auto setBackground(PrimP prim) {
        backgroundPrim_ = prim;
        return addPermanentWidget(prim);
    }

    auto setBackground(BackgroundWidgetP widget) {
        backgroundWidget_ = widget;
        return addPermanentWidget(widget);
    }


    auto setOrientation(M::Orientation orientation) {
        orientation_ = orientation;
        return updateLayout();
    }


private:
    DialogP onNew(DialogParamsA params);

    void updateVisible(bool visible);

protected:
    void onCreate(SceneP scene) override;
    void onVisible(bool visible) override { updateVisible(visible); }

    bool onTouchDown(TouchA touch) override;
    bool onTouchMove(TouchA touch) override;
    bool onTouchUp(TouchA touch) override;
};


} // namespace mangl
