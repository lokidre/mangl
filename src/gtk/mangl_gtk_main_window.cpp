/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_gtk_main_window.h"

#include <mangl/main.h>

#if MANGL_GTK


namespace mangl {

ManglMainWindow::ManglMainWindow() {
    fps_ = 30;
    // fps_ = 60 ;

#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#endif
}


// ManglMainWindow::~ManglMainWindow()
//{
// }

void ManglMainWindow::create(GtkApplication* app) {
    mangl_main();
    Framework::instance()->init();


    // Create a new window
    auto gwidget = gtk_application_window_new(app);
    auto gwindow = GTK_WINDOW(gwidget);
    gWindow = gwidget;


    gtk_window_set_title(gwindow, Env::appName.c_str());

    auto appRect = Env::applicationRect.convert<RectI>();

    if (appRect.w == M::InvalidCoord || appRect.h == M::InvalidCoord) {
        appRect.resize(Env::DefaultAppSize.convert<SizeI>());
    }

    auto screenSize = Env::physScreenSize.convert<SizeI>();
    auto windowSize = SizeI::make(appRect.w, appRect.h);

    auto desktopRect = RectI::make(0, 0, screenSize.w, screenSize.h);

    auto wndRect = desktopRect.centered(windowSize);

    if (appRect.x == M::InvalidCoordI || appRect.y == M::InvalidCoordI) {
        appRect.x = wndRect.x;
        appRect.y = wndRect.y;
    }

    Env::applicationRect = appRect.convert<Rect>();


    gtk_window_set_default_size(gwindow, wndRect.w, wndRect.h);


    auto glArea = gtk_gl_area_new();
    glArea_ = glArea;

    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(glArea), true);
    gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(glArea), true);
    // gtk_gl_area_set_auto_render(glArea, false);

    // gtk_container_add(GTK_CONTAINER(gWindow), glArea);


    //
    // Setup GL Area
    //
    auto box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    int boxMargin = 2;
    int marginTop = 0;
    int marginBottom = 16;
    gtk_widget_set_margin_start(box, boxMargin);
    gtk_widget_set_margin_end(box, boxMargin);
    gtk_widget_set_margin_top(box, marginTop);
    gtk_widget_set_margin_bottom(box, marginBottom);

    gtk_box_set_spacing(GTK_BOX(box), 0);
    gtk_window_set_child(GTK_WINDOW(gwindow), box);

    gtk_widget_set_hexpand(glArea, TRUE);
    gtk_widget_set_vexpand(glArea, TRUE);
    gtk_widget_set_size_request(
        glArea, (int)Env::applicationRect.w, (int)Env::applicationRect.h
    ); // TODO: size from window client area
    gtk_box_append(GTK_BOX(box), glArea);


    g_signal_connect(glArea, "render", G_CALLBACK(cbRender), this);
    g_signal_connect(glArea, "realize", G_CALLBACK(cbRealize), this);
    g_signal_connect(glArea, "unrealize", G_CALLBACK(unrealize_cb), this);


    motionEventController_ = gtk_event_controller_motion_new();
    g_signal_connect(motionEventController_, "enter", G_CALLBACK(mouse_enter_cb), this);
    g_signal_connect(motionEventController_, "leave", G_CALLBACK(mouse_leave_cb), this);
    g_signal_connect(motionEventController_, "motion", G_CALLBACK(mouse_move_cb), this);
    gtk_widget_add_controller(GTK_WIDGET(glArea_), motionEventController_);


    keyEventController_ = gtk_event_controller_key_new();
    g_signal_connect(keyEventController_, "key-pressed", G_CALLBACK(key_pressed_cb), this);
    // gtk_widget_add_controller(GTK_WIDGET(glArea_), keyEventController_);
    gtk_widget_add_controller(gwidget, keyEventController_);

    gestureClick_ = gtk_gesture_click_new();
    g_signal_connect(gestureClick_, "pressed", G_CALLBACK(gesture_pressed_cb), this);
    g_signal_connect(gestureClick_, "released", G_CALLBACK(gesture_released_cb), this);
    gtk_widget_add_controller(GTK_WIDGET(glArea_), GTK_EVENT_CONTROLLER(gestureClick_));


    gtk_window_present(gwindow);
    //gtk_widget_show(gwidget);

    // GtkAllocation widgetAllocation;
    // gtk_widget_get_allocation(gwidget, &widgetAllocation);


    auto fps = fps_;
    auto interval = 1000 / fps;
    // g_timeout_add_full(G_PRIORITY_HIGH, interval, timer_cb, mainWindow, NULL);
}

void ManglMainWindow::onMouseEnter(double x, double y) {
    // g_print("enter: %d, %d\n", (int)x, (int)y);
    // auto widget =
}

void ManglMainWindow::onMouseMove(double x, double y) {

    TouchInfo touch{
            coordToPoint(x, y),
            timestamp_,
    };

    if (mouseButtonDown_) {
        Framework::instance()->onTouchMove(touch);
    }

    lastTouches_ = touch;
}

void ManglMainWindow::onMouseLeave() {
    // g_print("leave\n");
}

void ManglMainWindow::onGesturePressed(int count, double x, double y) {
    mouseButtonDown_ = true;

    TouchInfo touch{coordToPoint(x, y), timestamp_};

    Framework::instance()->onTouchDown(touch);

    lastTouches_ = touch;
}
void ManglMainWindow::onGestureReleased(int count, double x, double y) {
    mouseButtonDown_ = false;

    TouchInfo touch{coordToPoint(x, y), timestamp_};

    Framework::instance()->onTouchUp(touch);
}


} // namespace mangl

#endif
