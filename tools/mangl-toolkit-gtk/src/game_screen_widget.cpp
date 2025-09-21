/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen_widget.h"

#include "main_application.hpp"

#include "pubsub.hpp"
#include "state.hpp"

#include <mangl/main_window.h>
#include <mangl/main.h>

namespace toolkit {

void GameScreenWidget::create() {
    Base::create("Game Screen");
    // gwidget_ = gtk_frame_new("Game Screen");
    setSizeRequest(320, 480);
    setHExpand(true)->setVExpand(true);
    //    gtk_widget_set_size_request(gwidget_, 320, 480);
    //    gtk_widget_set_hexpand(gwidget_, true);
    //    gtk_widget_set_vexpand(gwidget_, true);

    setChild(layout_.create()); // = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, true);
    // gtk_frame_set_child(GTK_FRAME(gwidget_), GTK_WIDGET(layout_));

    gw::Spacer vspacer{gw::Orientation::Vertical};
    layout_.append(vspacer.create());


    hbox_.create()->setHExpand();
    layout_.append(&hbox_);


    //    auto vspacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, false);
    //    gtk_widget_set_vexpand(vspacer, true);
    //    gtk_box_append(layout_, vspacer);


    //    hbox_ = (GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL, true);
    //    gtk_widget_set_hexpand(GTK_WIDGET(hbox_), true);
    //    gtk_box_append(layout_, GTK_WIDGET(hbox_));

    gw::Spacer hspacer{gw::Orientation::Horizontal};
    hbox_.append(hspacer.create());

    //    auto hspacer2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, false);
    //    gtk_widget_set_hexpand(hspacer2, true);
    //    hbox_.gappend(hspacer2);


    fixed_ = (GtkFixed*)gtk_fixed_new();
    hbox_.gappend(fixed_);


    backgroundWidget_ = (GtkDrawingArea*)gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(backgroundWidget_, Self::area_draw_cb, this, nullptr);
    // gtk_box_append(hbox_, GTK_WIDGET(backgroundWidget_));
    gtk_fixed_put(fixed_, GTK_WIDGET(backgroundWidget_), 0, 0);


    //
    // GLArea initialization
    //
    glArea_ = (GtkGLArea*)gtk_gl_area_new();
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(glArea_), true);
    gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(glArea_), true);

    gtk_fixed_put(fixed_, GTK_WIDGET(glArea_), 0, 0);

    mangl_main();
    Framework::instance()->init();

    g_signal_connect(glArea_, "render", G_CALLBACK(gl_render_cb), this);
    g_signal_connect(glArea_, "realize", G_CALLBACK(gl_realize_cb), this);
    g_signal_connect(glArea_, "unrealize", G_CALLBACK(gl_unrealize_cb), this);


    hbox_.append(hspacer.create());

    //    hspacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, false);
    //    gtk_widget_set_hexpand(hspacer, true);
    //    hbox_.gappend(hspacer);

    //    vspacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, false);
    //    gtk_widget_set_vexpand(vspacer, true);
    //    gtk_box_append(layout_, vspacer);

    layout_.append(vspacer.create());


    auto sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    layout_.gappend(sep);

    timeScale_ = (GtkScale*)gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(GTK_WIDGET(timeScale_), 150, -1);
    gtk_scale_set_digits(timeScale_, 1);
    gtk_scale_set_draw_value(timeScale_, true);
    layout_.gappend(timeScale_);

    updateLayout();


    motionEventController_ = gtk_event_controller_motion_new();
    g_signal_connect(motionEventController_, "enter", G_CALLBACK(mouse_enter_cb), this);
    g_signal_connect(motionEventController_, "leave", G_CALLBACK(mouse_leave_cb), this);
    g_signal_connect(motionEventController_, "motion", G_CALLBACK(mouse_move_cb), this);
    gtk_widget_add_controller(GTK_WIDGET(glArea_), motionEventController_);

    keyEventController_ = gtk_event_controller_key_new();
    g_signal_connect(keyEventController_, "key-pressed", G_CALLBACK(key_pressed_cb), this);
    // gtk_widget_add_controller(GTK_WIDGET(glArea_), keyEventController_);
    gtk_widget_add_controller(
        GTK_WIDGET(MainApplication::instance()->mainWindow.gwidget()), keyEventController_
    );

    gestureClick_ = gtk_gesture_click_new();
    g_signal_connect(gestureClick_, "pressed", G_CALLBACK(gesture_pressed_cb), this);
    g_signal_connect(gestureClick_, "released", G_CALLBACK(gesture_released_cb), this);
    gtk_widget_add_controller(GTK_WIDGET(glArea_), GTK_EVENT_CONTROLLER(gestureClick_));

    // frameWidget_ = gtk_frame_new("");

    auto pubsub = Pubsub::instance();

    pubsub->mainWindowDestroy = [this]{ onMainWindowDestroy(); };

    // opengl_.init();
}


void GameScreenWidget::updateLayout() {
    auto state = State::instance();
    auto screenWidth = (int)state->screenSize.w;
    auto screenHeight = (int)state->screenSize.h;

    gtk_widget_set_size_request(GTK_WIDGET(fixed_), screenWidth, screenHeight);
    gtk_widget_set_size_request(GTK_WIDGET(glArea_), screenWidth, screenHeight);

    gtk_drawing_area_set_content_width(backgroundWidget_, screenWidth);
    gtk_drawing_area_set_content_height(backgroundWidget_, screenHeight);
}


void GameScreenWidget::onBackgroundDraw(cairo_t* cr, int width, int height) {

    // auto context = gtk_widget_get_style_context(GTK_WIDGET(backgroundWidget_));
    cairo_rectangle(cr, 0, 0, width, height);

    GdkRGBA lightColor, darkColor;
    gdk_rgba_parse(&lightColor, "#F5F5DC");
    gdk_rgba_parse(&darkColor, "#202020");

    gdk_cairo_set_source_rgba(cr, &lightColor);
    cairo_fill(cr);

    gdk_cairo_set_source_rgba(cr, &darkColor);

    auto chsize = State::instance()->checkboardSize;
    auto rows = height / chsize + 1;
    auto cols = width / chsize + 1;

    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            if ((i & 1) == (j & 1)) {
                cairo_rectangle(cr, i * chsize, j * chsize, chsize, chsize);
                cairo_fill(cr);
            }
        }
    }
}


bool GameScreenWidget::onGlRender() {
    using namespace mangl;

    if (!timerInitialized_)
        return true;

    try {
        Framework::getRenderer()->prepareScene();
        Framework::getRenderer()->onRender();
    } catch (const std::exception& e) {
        mmCallbackError(e.what());
        return false;
    } catch (...) {
        mmCallbackError("MM Callback: Unhandled Exception!");
        return false;
    }


    return true;
}

void GameScreenWidget::onGlRealize() {

    gtk_gl_area_make_current(glArea_);
    auto result = gtk_gl_area_get_error(glArea_);

    opengl_.init();
    Framework::instance()->onInitialized();


    auto fps = fps_;
    auto interval = 1000 / fps;
    g_timeout_add_full(G_PRIORITY_HIGH, interval, Self::timer_cb, this, nullptr);
}

void GameScreenWidget::onGlUnrealize() { opengl_.destroy(); }

static inline uint64_t getTimestamp() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


bool GameScreenWidget::onTimer() {

    try {
        if (gameLoopNeedExit_) {
            return false;
        }

        if (!timerInitialized_) {
            auto fw_ = Framework::instance();
            fw_->onStart();
            // auto renderer = fw_->getRenderer();
            fw_->onViewport();

            lastTimerCounter = getTimestamp();

            currentTimestamp = 0;
            timestampDelta = 0;

            fw_->onInitialized();
            timerInitialized_ = true;
        }

        currentTimerCounter = getTimestamp();

        uint64_t elapsedInt = currentTimerCounter - lastTimerCounter;
        Timestamp elapsed = Timestamp(elapsedInt) / 1000;


#if MANGL_DEBUG_INCREMENTAL_TIMER
        currentTimestamp_ += elapsed < timestampPeriod_ ? elapsed : timestampPeriod_;
#else
        currentTimestamp += elapsed;
#endif

        timestamp_ = currentTimestamp;

        {
            auto fw_ = Framework::instance();
            auto renderer = Framework::getRenderer();

            fw_->processTimer(timestamp_);
            renderer->onTimer(timestamp_);

            // renderer->prepareScene();
            // renderer->onRender();

            // throw std::runtime_error("test error");
        }

        gtk_widget_queue_draw(GTK_WIDGET(glArea_));
    } catch (const std::exception& e) {
        mmCallbackError(e.what());
        return false;
    } catch (...) {
        mmCallbackError("MM Callback: Unhandled Exception!");
        return false;
    }

    return true;
}


void GameScreenWidget::mmCallbackError(const char* err) {
    gameLoopNeedExit_ = true;
    timerInitialized_ = false;

    // auto flags = (GtkDialogFlags)(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL);
    auto parent = MainApplication::instance()->mainWindow.gwindow();
    // auto type = GTK_MESSAGE_ERROR;
    // auto buttons = GTK_BUTTONS_OK;

    const char *buttons[] = {"OK", nullptr};

    auto dialog = gtk_alert_dialog_new("Error!");
    //gtk_alert_dialog_set_detail(dialog, err);
    gtk_alert_dialog_set_message(dialog, err);
    gtk_alert_dialog_set_modal(dialog, true);
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);

    gtk_alert_dialog_show(dialog, parent);



    /*
    auto dialog = gtk_message_dialog_new(parent, flags, type, buttons, "Error!");

    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", err);

    gtk_window_present(GTK_WINDOW(dialog));

    g_signal_connect(dialog, "response", G_CALLBACK(Self::error_dialog_cb), this);
    */
    // exit(1);
}

void GameScreenWidget::onMainWindowDestroy() { gameLoopNeedExit_ = true; }

void GameScreenWidget::onMouseEnter(double x, double y) {
    // g_print("enter: %d, %d\n", (int)x, (int)y);
    // auto widget =
}

void GameScreenWidget::onMouseMove(double x, double y) {
    TouchInfo touch{
        .p = coordToPoint(x, y),
        .timestamp = timestamp_,
    };

    if (mouseButtonDown_) {
        Framework::instance()->onTouchMove(touch);
    }

    lastTouches_ = touch;
}

void GameScreenWidget::onMouseLeave() {
    // g_print("leave\n");
}

void GameScreenWidget::onGesturePressed(int count, double x, double y) {
    mouseButtonDown_ = true;

    TouchInfo touch{
        coordToPoint(x, y),
        timestamp_,
    };

    Framework::instance()->onTouchDown(touch);

    lastTouches_ = touch;
}
void GameScreenWidget::onGestureReleased(int count, double x, double y) {
    mouseButtonDown_ = false;

    TouchInfo touch{
        .p = coordToPoint(x, y),
        .timestamp = timestamp_,
    };

    Framework::instance()->onTouchDown(touch);
}

}  // namespace toolkit
