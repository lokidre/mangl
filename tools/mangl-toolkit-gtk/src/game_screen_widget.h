/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/environment.h>
#include "main_types.hpp"
#include "game_screen_opengl.h"
#include "gwidgets/widgets.hpp"

namespace toolkit {

class GameScreenWidget: public gw::Frame {
    using Base = gw::Frame;
    using Self = GameScreenWidget;
    using SelfP = Self*;

public:
    void create();

private:
    GtkDrawingArea* backgroundWidget_{};
    GtkFixed* fixed_{};
    GtkGLArea* glArea_{};

    gw::VBoxLayout layout_;
    gw::HBoxLayout hbox_;

    GtkEventController *motionEventController_{}, *keyEventController_{};
    GtkGesture* gestureClick_{};
    GtkScale* timeScale_{};

    void updateLayout();

    GameScreenOpenGL opengl_;


    bool timerInitialized_{};
    Timestamp timestamp_{};

    bool gameLoopNeedExit_{};
    Timestamp currentTimestamp{}, timestampDelta{}, timestampPeriod{};
    uint64_t lastTimerCounter{}, currentTimerCounter{};
    int fps_{30};

    void mmCallbackError(const char* err);

    void onMainWindowDestroy();


public:
    void onBackgroundDraw(cairo_t* cr, int width, int height);
    bool onGlRender();
    void onGlRealize();
    void onGlUnrealize();
    bool onTimer();

private:
    static gboolean gl_render_cb(GtkGLArea*, GdkGLContext* context, gpointer userData) {
        return static_cast<SelfP>(userData)->onGlRender();
    }

    static void gl_realize_cb(GtkGLArea* area, gpointer userData) {
        gtk_gl_area_make_current(area);

        if (gtk_gl_area_get_error(area))
            return;

        static_cast<SelfP>(userData)->onGlRealize();
    }

    static void gl_unrealize_cb(GtkWidget* widget, gpointer userData) {
        gtk_gl_area_make_current(GTK_GL_AREA(widget));

        if (gtk_gl_area_get_error(GTK_GL_AREA(widget)))
            return;

        static_cast<SelfP>(userData)->onGlUnrealize();
    }

    static gboolean timer_cb(gpointer userData) { return static_cast<SelfP>(userData)->onTimer(); }

    static void
    area_draw_cb(GtkDrawingArea*, cairo_t* cr, int width, int height, gpointer userData) {
        static_cast<SelfP>(userData)->onBackgroundDraw(cr, width, height);
    }

    static void error_dialog_cb(GtkDialog* dialog, gint responseId, gpointer userData) {

        if (responseId == GTK_RESPONSE_OK || responseId == GTK_RESPONSE_CLOSE) {

            // int a =0;
        }

        if (responseId != GTK_RESPONSE_DELETE_EVENT) {
            gtk_window_destroy(GTK_WINDOW(dialog));
        }
    }


    bool mouseButtonDown_{};
    TouchInfo lastTouches_{};

    [[nodiscard]] auto coordToPoint(double x, double y) const noexcept {
        auto p = Point::make(x, y);
        p /= Env::screenScale;

        const auto &layout = Env::layoutSize;
        p.y = layout.h - 1 - p.y;

        return p;
    }


    void onMouseEnter(double x, double y);
    void onMouseMove(double x, double y);
    void onMouseLeave();

    void onGesturePressed(int count, double x, double y);
    void onGestureReleased(int count, double x, double y);


    static void mouse_enter_cb(GtkEventControllerMotion*, gdouble x, gdouble y, gpointer userData) {
        static_cast<SelfP>(userData)->onMouseEnter(x, y);
    }

    static void mouse_move_cb(GtkEventControllerMotion*, gdouble x, gdouble y, gpointer userData) {
        static_cast<SelfP>(userData)->onMouseMove(x, y);
    }

    static void mouse_leave_cb(GtkEventControllerMotion*, gpointer userData) {
        static_cast<SelfP>(userData)->onMouseLeave();
    }


    static gboolean key_pressed_cb(
        GtkEventControllerKey*, guint keyval, guint keycode, GdkModifierType state,
        gpointer userData
    ) {
        g_print("key-pressed: %d, %d, %d\n", keyval, keycode, state);
        return false;
    }

    static void gesture_pressed_cb(
        GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer userData
    ) {
        //g_print("pressed: %d, %d, %d\n", n_press, (int)x, (int)y);
        static_cast<SelfP>(userData)->onGesturePressed(n_press, x, y);
    }

    static void gesture_released_cb(
        GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer userData
    ) {
        //g_print("released: %d, %d, %d\n", n_press, (int)x, (int)y);
        static_cast<SelfP>(userData)->onGestureReleased(n_press, x, y);
    }
};

}  // namespace toolkit
