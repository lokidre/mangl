/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"

#if MANGL_GTK


#include "../core/mangl_types.h"
#include "../core/mangl_framework.h"
#include "../core/mangl_storage.h"

#include <gtk/gtk.h>


namespace mangl {

class ManglMainWindow: public apl::Singleton<ManglMainWindow, false> {
    using Self = ManglMainWindow;
    using SelfP = Self*;

    Setting<PointI> settingWindowTopLeft{"mangl.win.main_window.top_left"};

public:
    GtkWidget* gWindow{};
    GtkWidget* glArea_{};
    Timestamp fps_;

    GtkEventController* motionEventController_{};
    GtkEventController* keyEventController_{};
    GtkGesture* gestureClick_{};


    
public:
    ManglMainWindow();
    //~ManglMainWindow() override;

    void create(GtkApplication* app);

#if MANGL_OPENGL
    void initOpenGL();
    void releaseOpenGL();
#endif

    bool onTimer();
    bool onGlRender();
    void onGlRealize(GtkGLArea* area);
    void onGlUnrealize(GtkWidget* widget);

    static gboolean cbRender(GtkGLArea*, GdkGLContext*, gpointer userData) {
        return static_cast<SelfP>(userData)->onGlRender();
    }

    static void cbRealize(GtkGLArea* area, gpointer userData) {
        static_cast<SelfP>(userData)->onGlRealize(area);
    }

    static void unrealize_cb(GtkWidget* widget, gpointer userData) {
        static_cast<SelfP>(userData)->onGlUnrealize(widget);
    }


    static gboolean timer_cb(gpointer userData) {
        return static_cast<SelfP>(userData)->onTimer();
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
        static_cast<SelfP>(userData)->onGesturePressed(n_press, x, y);
    }

    static void gesture_released_cb(
        GtkGestureClick* self, gint n_press, gdouble x, gdouble y, gpointer userData
    ) {
        static_cast<SelfP>(userData)->onGestureReleased(n_press, x, y);
    }

    bool mouseButtonDown_{};
    TouchInfo lastTouches_{};

    [[nodiscard]] static auto coordToPoint(double x, double y) noexcept {
        auto p = Point::make(x, y);

        p /= Env::screenScale;

        const auto &layout = Env::layoutSize;
        p.y = layout.h - 1 - p.y;

        return p;
    }


private:
    bool gameLoopNeedExit_{};
    bool timerInitialized_{};
    Timestamp timestamp_{};

    int oldFramebuffer_{}, framebuffer_{}, colorRenderbuffer_{}, depthRenderbuffer_{};
    bool buffersAllocated_{};

    void mmCallbackError(const char* err);

};

}

#endif
