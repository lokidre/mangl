/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_dialog.h"
#include "mangl_grid_layout.h"

#include "../native/mangl_native_gui.h"

#include "../mangl_framework.h"
#include "../mangl_styles.h"


namespace mangl {

DialogP Scene::newDialog(DialogParamsA params) {
    return newWidget<Dialog>(params.flags)->onNew(params)->create();
}


DialogP Dialog::onNew(DialogParamsA params) {

    params_ = params;

    titleText_ = params.title;
    messageText_ = params.message;

    confirmButtonText_ = params.confirmButtonText;
    confirmSignal_ = params.confirmSignal;
    if (params.onConfirm) confirmSignal_ = params.onConfirm;

    rejectButtonText_ = params.rejectButtonText;
    rejectSignal_ = params.rejectSignal;
    if (params.onReject) rejectSignal_ = params.onReject;

    if (params_.rect.w != 0 && params_.rect.h != 0) {
        move(params_.rect);
    }

    updateVisible(false);

    return this;
}


void Dialog::Item::setVisible(bool flag) {
    bool v = active && flag;

    if (prim) prim->setVisible(v);
    if (widget) widget->setVisible(v);
    //if (nativeControlId)
    //    MNativeGui::instance()->setVisible(nativeControlId, v);
}


void Dialog::setConfirmSignal(SlotVoid signal) {
    confirmSignal_ = std::move(signal);

    if (confirmButton_) {
        confirmButton_->onPush = [this] {
            if (confirmSignal_) confirmSignal_();
            if (closeOnAction_) close();
            if (params_.dispose) scene_->deleteWidget(this);
        };
    }
}

void Dialog::setRejectSignal(SlotVoid signal) {
    rejectSignal_ = std::move(signal);

    if (rejectButton_) {
        rejectButton_->onPush = [this] {
            if (rejectSignal_) rejectSignal_();
            //params_.rejectSignal();
            if (closeOnAction_) close();
            if (params_.dispose) scene_->deleteWidget(this);
        };
    }

}


DialogP Dialog::clear() {
    if (active_)
        close();

    active_ = false;

    closeOnAction_ = false;
    r_ = {};

    return this;
}

void Dialog::onCreate(SceneP scene) {
    auto& theme = params_.theme ? *params_.theme : M::Styles::dialog;

    bool needBackground = !backgroundPrim_ && !backgroundWidget_;

    if (needBackground) {
        backgroundStackWidget_ = scene->newStackWidget();

        if (theme.backgroundWidgetParams.valid()) {
            auto widget = scene->newBackgroundWidget(theme.backgroundWidgetParams);
            backgroundStackWidget_->addWidget(widget);

        }  else {
            auto prim = scene->newBox(theme.backgroundColor, {}, M::Hidden);

            if (theme.border.radius > 0) {
                prim->setRadius(theme.border.radius);
            }

            backgroundStackWidget_->addPrim(prim);
        }

        if (theme.decorCreateCallback) {
            auto decorWidget = theme.decorCreateCallback(this);
            backgroundStackWidget_->addWidget(decorWidget);
        }

        if (params_.grid) {
            //params_.grid->setZIndexFront();
            scene->setZIndexFront(params_.grid);
        }
    }

    if (!theme.border.strikes.empty()) {
        borderRuntime_.assign(theme.border).create(scene, 0).setVisible(false);
    }

    updateVisible(false);

    if (params_.run)
        run();
}

void Dialog::updateVisible(bool visible)
{
    borderRuntime_.setVisible(visible);

    if (backgroundPrim_) backgroundPrim_->setVisible(visible);
    if (backgroundWidget_) backgroundWidget_->setVisible(visible);
    if (backgroundStackWidget_) backgroundStackWidget_->setVisible(visible);

    if (params_.grid) params_.grid->setVisible(visible);

    for (auto &item: permanentItems_)
        item.setVisible(visible);

    for (auto &item: items_)
        item.setVisible(visible);
}

void Dialog::show(bool nonmodal)
{
    auto scene = this->scene_;
    auto& theme = params_.theme ? *params_.theme : M::Styles::dialog;

    switch(params_.type) {
    case DialogType::Confirm:
        haveConfirmButton_ = true;
        break;
    case DialogType::YesNo:
        haveConfirmButton_ = true;
        haveRejectButton_ = true;
        break;
    default:
        break;
    }

    if (haveConfirmButton_) {
        if (!confirmButton_) {
            auto button = scene->newButton({
                .style = &theme.button,
                .text = confirmButtonText_,
                .flags = M::Hidden,
            });

            setConfirmButton(button);
            setConfirmSignal(confirmSignal_);
        }
    } else {
        if (confirmButton_) confirmButton_->clearText()->hide();
    }

    if (haveRejectButton_) {
        if (!rejectButton_) {
            auto button = scene->newButton({
                .style = &theme.button,
                .text = rejectButtonText_,
                .flags = M::Hidden,
            });

            setRejectButton(button);
            setRejectSignal(rejectSignal_);
        }
    } else {
        if (rejectButton_) rejectButton_->clearText()->hide();
    }


    if (!titleText_.empty() && !titleLabel_) {
        auto label = scene->newLabel({
            .text = titleText_,
            .color = theme.titleText.color,
            .textSize = theme.titleText.size,
            .flags = M::AlignCenter | M::AlignTop | M::Hidden,
        });

        setTitleLabel(label);
    }

    if (!messageText_.empty() && !messageLabel_) {
        auto label = scene->newLabel({
            .text = messageText_,
            .color = theme.contentsText.color,
            .textSize = theme.contentsText.size,
            .flags = M::AlignCenter | M::AlignVCenter | M::Hidden,
        });
        setMessageLabel(label);
    }



    // figure out inactive items
    for (auto &item: permanentItems_) {
        if (!item.widget)
            continue;

        if (item.widget == titleLabel_)
            item.active = !titleLabel_->empty();

        if (item.widget == messageLabel_)
            item.active = !messageLabel_->empty();

        if (item.widget == confirmButton_)
            item.active = haveConfirmButton_;

        if (item.widget == rejectButton_)
            item.active = haveRejectButton_;
    }

    interactiveWidgets_.clear();
    for (auto &item: permanentItems_) {
        if (item.active && item.widget && !item.widget->interactionDisabled_)
            interactiveWidgets_.push_back(item.widget);
    }
    for (auto &item: items_) {
        if (item.active && item.widget && !item.widget->interactionDisabled_)
            interactiveWidgets_.push_back(item.widget);
    }

    if (params_.grid) {
        for (auto& item: params_.grid->getItems())
            if (item.widget && !item.widget->interactionDisabled_)
                interactiveWidgets_.push_back(item.widget);
        //scene->setZIndexFront(params_.grid);
    }

    // Create all the elements
    for (auto &item: permanentItems_)
        if (item.active && item.widget)
            item.widget->create();


    setVisible(true);
    updateLayout();

    if (!params_.modeless && !nonmodal)
        scene_->setModalWidget(this);

    active_ = true;
}

void Dialog::run()
{
    closeOnAction_ = true;
    show();
}

void Dialog::runModless() {
    closeOnAction_ = true;
    show(true);
}


void Dialog::updateLayout() {

    auto& theme = params_.theme ? *params_.theme : M::Styles::dialog;

    auto flipped = M::Orientation180 == orientation_;

    auto dialogSize = userLayout_.size();

    Size contentSize{};
    Margins contentMargins{};

    if (messageLabel_ && !messageLabel_->empty()) {
        contentSize.h += theme.spacing.h * 0.5;
        contentSize.h += messageLabel_->getTextHeight();
        contentSize.h += theme.spacing.h * 0.5;
    }

    if (params_.grid) {
        contentSize.h += theme.spacing.h * 0.5;
        auto gridSize = params_.grid->estimateSize();
        contentSize.h += gridSize.h;
        contentSize.h += theme.spacing.h * 0.5;
    }


    auto contentsTop = theme.margins.top() + theme.padding.top();

    if (titleLabel_ && !titleLabel_->empty()) {
        contentsTop += titleLabel_->getTextHeight();
        contentsTop += theme.spacing.h;
    }
    if (contentsTop < theme.contentsPadding.top())
        contentsTop = theme.contentsPadding.top();


    auto contentsBottom = theme.margins.bottom() + theme.padding.bottom();
    contentsBottom += theme.button.size.h;
    contentsBottom += theme.spacing.h;

    // Make it symmetrical
    if (contentsTop < contentsBottom) contentsTop = contentsBottom;

    if (contentsBottom < theme.contentsPadding.bottom())
        contentsBottom = theme.contentsPadding.bottom();

    contentMargins.setTop(contentsTop);
    contentMargins.setBottom(contentsBottom);


    if (dialogSize.w == 0) {
        dialogSize.w = theme.smallSize.w;

        dialogSize.h = contentSize.h + contentMargins.vert2();

        auto layout = Env::getSafeArea();

        auto dialogVertK = flipped ? 0.4 : 0.6;

        auto rect = layout.positioned(dialogSize, {0.5, dialogVertK});
        r_ = rect;
    }

    borderRuntime_.move(r_);

    if (backgroundPrim_) backgroundPrim_->move(r_);

    if (backgroundWidget_) backgroundWidget_->move(r_);
    if (backgroundStackWidget_) backgroundStackWidget_->move(r_);


    auto dialogCenter = this->r_.center();

    Cord currentY{};

    auto advanceY = [&currentY, &flipped](Cord y, Cord y2 = 0) {
        if (flipped) {
            currentY += y;
            currentY += y2;
        } else {
            currentY -= y;
            currentY -= y2;
        }
    };

    currentY = flipped ? r_.bottom() : r_.top();

    advanceY(theme.margins.top(), theme.padding.top());

    if (titleLabel_ && !titleLabel_->empty()) {
        auto vertAlign = flipped ? M::AlignBottom : M::AlignTop;
        titleLabel_->setTextAlignment(M::AlignCenter | vertAlign);
        titleLabel_->setPos(dialogCenter.x, currentY);
        titleLabel_->setOrientation(orientation_);

        advanceY(titleLabel_->getTextHeight(), theme.spacing.h);
    }

    currentY = flipped ? r_.bottom() + contentsTop : r_.top() - contentsTop;

    if (flipped) {
    } else {
        if (this->r_.top() - currentY < theme.contentsPadding.top()) {
            currentY = this->r_.top() - theme.contentsPadding.top();
            currentY -= theme.spacing.h/2;
        }
    }

    if (messageLabel_ && !messageLabel_->empty()) {
        auto messageTextHeight = messageLabel_->getTextHeight();

        if (params_.grid) {
            Point pos{dialogCenter.x, currentY - messageTextHeight/2};
            messageLabel_->setPos(pos);
            advanceY(messageTextHeight, theme.spacing.h);
        } else {
            Rect availRect = r_;

            if (flipped) {
                availRect.shrinkY(contentMargins.top());
                availRect.shrinkH(contentMargins.bottom());
            } else {
                availRect.shrinkH(contentMargins.top());
                availRect.shrinkY(contentMargins.bottom());
            }

            messageLabel_->setPos(availRect.center());
        }

        messageLabel_->setOrientation(orientation_);
    }


    if (params_.grid) {
        auto gridSize = params_.grid->estimateSize();
        auto left = theme.margins.left() + theme.padding.left();
        if (left < theme.contentsPadding.left())
            left = theme.contentsPadding.left();

        auto right = theme.margins.right() + theme.padding.right();
        if (right < theme.contentsPadding.right())
            right = theme.contentsPadding.right();

        params_.grid->move({r_.x + left, currentY - gridSize.h, r_.w - right - left, gridSize.h});
        advanceY(gridSize.h, theme.spacing.h);
    }


    //
    // Buttons
    //
    int buttonsCount = 0;
    if (confirmButton_ && haveConfirmButton_)
        ++buttonsCount;
    if (rejectButton_ && haveRejectButton_)
        ++buttonsCount;


    auto contentWidth = this->r_.w - theme.margins.horz2();

    auto buttonRect = Rect::make({}, theme.button.size);

    auto calcButtonY = [this, &theme, flipped] {
        Cord y{};

        if (flipped) {
            y = this->r_.top() - theme.margins.bottom() - theme.padding.bottom();
            y -= theme.button.size.h;
        } else {
            y = this->r_.y + theme.margins.bottom() + theme.padding.bottom();
        }

        return y;
    };

    auto calcButtonLeft = [this, &theme, contentWidth, &buttonRect] {
        return this->r_.x + theme.margins.left() + (contentWidth/2 - buttonRect.w)/2;
    };

    auto calcButtonRight = [this, &theme, &contentWidth, &buttonRect] {
        return this->r_.right() - theme.margins.right() - (contentWidth/2 - buttonRect.w)/2 - buttonRect.w;
    };

    if (confirmButton_ && haveConfirmButton_) {
        buttonRect.y = calcButtonY();

        switch (buttonsCount) {
        case 1:
            buttonRect.x = this->r_.center().x - buttonRect.w/2;
            break;
        case 2: 
            buttonRect.x = flipped ? calcButtonLeft() : calcButtonRight();
            break;
        default:
            break;
        }

        confirmButton_->move(buttonRect)->setOrientation(orientation_);
    }

    if (rejectButton_ && haveRejectButton_) {
        buttonRect.y = calcButtonY();
        buttonRect.x = flipped ? calcButtonRight() : calcButtonLeft();

        rejectButton_->move(buttonRect)->setOrientation(orientation_);
    }

}


void Dialog::close() {
    scene_->setModalWidget(nullptr);

    setVisible(false);

    active_ = false;

    items_.clear();
}




bool Dialog::onTouchDown(TouchA touch)
{
    if (!active_)
        return false;

    for (auto widget: interactiveWidgets_) {
        if (widget->disabled() || !widget->visible_)
            continue;
        if (widget->onTouchDown(touch))
            return true;
    }

    if (this->r_.contains(touch.p))
        return true;

    return false;
}

bool Dialog::onTouchMove(TouchA touch)
{
    if (!active_)
        return false;

    for (auto widget: interactiveWidgets_) {
        if (widget->disabled() || !widget->visible())
            continue;
        if (widget->onTouchMove(touch))
            return true;
    }

    if (this->r_.contains(touch.p))
        return true;

    return false;
}


bool Dialog::onTouchUp(TouchA touch)
{
    if (!active_)
        return false;

    for (auto widget: interactiveWidgets_) {
        if (widget->disabled() || !widget->visible_)
            continue;
        if (widget->onTouchUp(touch))
            return true;
    }

    if (this->r_.contains(touch.p))
        return true;

    return false;
}


}  // namespace mangl

