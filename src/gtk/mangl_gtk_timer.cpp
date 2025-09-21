/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_gtk_main_window.h"


namespace mangl {

static Timestamp currentTimestamp{}, timestampDelta{}, timestampPeriod{};
static uint64_t lastTimerCounter{}, currentTimerCounter{};
static Framework* fw_{};
static SceneRenderer* renderer{};



static inline uint64_t getTimestamp() {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

bool ManglMainWindow::onTimer() {

    try {
        if (gameLoopNeedExit_) {

#if MANGL_OPENGL
            releaseOpenGL();
#endif
            return false;
        }

        if (!timerInitialized_) {

#if MANGL_OPENGL
            initOpenGL();
#endif
            timerInitialized_ = true;

            //
            // Main Message cycle
            //
            fw_ = Framework::instance();
            fw_->onStart();
            renderer = Framework::getRenderer();
            fw_->onViewport();

            lastTimerCounter = getTimestamp();

            currentTimestamp = 0;
            timestampDelta = 0;

            //timerInitialized_ = true;


            fw_->onInitialized();
        }

        currentTimerCounter = getTimestamp();

        uint64_t elapsedInt = currentTimerCounter - lastTimerCounter; //) * 1000;
        Timestamp elapsed = Timestamp(elapsedInt) / 1000;


#if MANGL_DEBUG_INCREMENTAL_TIMER
        currentTimestamp_ += elapsed < timestampPeriod_ ? elapsed : timestampPeriod_;
#else
        currentTimestamp += elapsed;
#endif

        timestamp_ = currentTimestamp;
        fw_->processTimer(timestamp_);
        renderer->onTimer(timestamp_);

        lastTimerCounter = currentTimerCounter;



        //gtk_widget_queue_draw(gWindow);
        gtk_widget_queue_draw(glArea_);
    } catch (const std::exception& e) {
        mmCallbackError(e.what());
        return false;
    } catch (...) {
        mmCallbackError("MM Callback: Unhandled Exception!");
        return false;
    }

    return true;
}

void ManglMainWindow::mmCallbackError(const char* err) {

    const char *buttons[] = {"OK", nullptr};

    auto dialog = gtk_alert_dialog_new("Error!");
    //gtk_alert_dialog_set_detail(dialog, err);
    gtk_alert_dialog_set_message(dialog, err);
    gtk_alert_dialog_set_modal(dialog, true);
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);

    gtk_alert_dialog_choose(dialog, nullptr, nullptr, nullptr, nullptr);


    // GtkDialogFlags flags = GTK_DIALOG_MODAL;
    // //GTK_DIALOG_DESTROY_WITH_PARENT |
    //
    // //auto msg = gtk_message_dialog_new(gWindow, flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error!");
    //
    // auto msg = gtk_message_dialog_new(GTK_WINDOW(gWindow), flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", err);
    //
    // //gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(msg), err);
    //
    // //auto response = gtk_dialog_run(GTK_DIALOG(msg));
    //
    // g_signal_connect(msg, "response", G_CALLBACK(gtk_window_destroy), NULL);

    exit(1);
}


} // namespace mangl
