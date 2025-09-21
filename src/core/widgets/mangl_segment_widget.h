/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_widget.h"
#include "../mangl_texture.h"
#include "../mangl_storage.h"

#include "mangl_label.h"
#include "mangl_segment_widget_params.h"


namespace mangl {

class SegmentWidget: public WidgetTemplate<SegmentWidget> {
    friend class Segmented;

protected:
    virtual void onSetParams(const SegmentWidgetParams& iparams) {}
    virtual void onSetIndex(int index, int total) {}
    virtual void onSetSelected(bool selected) {}
    virtual void onSetPressed(bool pressed) {}
};


class DefaultSegmentWidget: public SegmentWidget {
    SegmentWidgetParams params_{};
    int index_{}, total_{};

    PrimP prim_{};


    struct TextureSet {
        Texture normal, pressed, selected;

        void set(TextureA tex) {
            if (!tex.valid())
                return;
            normal = tex.getFrame(0);
            pressed = tex.getFrame(1);
            selected = tex.getFrame(2);
        }
    };

    TextureSet currTex{}, normalTex{}, firstTex{}, lastTex {};

    LabelP textLabel_{};
    bool pressed_{}, selected_ {};
    WidgetP currentWidget_ {};

    void onSetParams(const SegmentWidgetParams& iparams) override {
        params_ = iparams;
    }

    void onSetIndex(int index, int total) override {
        index_ = index;
        total_ = total;

        if (prim_) {
            if (index == 0) {
                if (firstTex.normal)
                    currTex = firstTex;
            } else if (index == total - 1) {
                if (lastTex.normal)
                    currTex = lastTex;
            } else {
                currTex = normalTex;
            }
        }

        updateStatus();
    }

    void onSetSelected(bool selected) override {
        if (selected != selected_) {
            selected_ = selected;
            updateStatus();
        }
    }

    void onSetPressed(bool pressed) override {
        if (pressed != pressed_) {
            pressed_ = pressed;
            updateStatus();
        }
    }


    void updateLayout();
    void updateStatus();
    void updateVisible();

    void switchWidget(WidgetP widget) {
        if (currentWidget_) currentWidget_->setVisible(false);
        currentWidget_ = widget;
        if (widget) currentWidget_->setVisible(true)->move(this->r_);
    }

    void switchTexture(const TextureSet& set) {
    }



protected:
    void onCreate() override;
    void onMove() override { updateLayout(); };
    void onVisible(bool visible) override { updateVisible(); };
    void onAlpha() override;


};


}  // namespace mangl

