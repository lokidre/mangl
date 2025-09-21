/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_gtk_main_window.h"

#if MANGL_GTK && MANGL_OPENGL

#include <gtk/gtk.h>

#include "../core/opengl/mangl_opengl.h"


namespace mangl {

bool ManglMainWindow::onGlRender() {
    if (!timerInitialized_)
        return true;

//    GLint defaultId{};
//    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultId);
//    glBindFramebuffer(GL_FRAMEBUFFER, defaultId);  ManglAssertOpenGL();


    //    glClearColor(1, 0, 0, 1);
    //    glClear(GL_COLOR_BUFFER_BIT);

    auto renderer = Framework::getRenderer();

    renderer->prepareScene();
    renderer->onRender();


//    int width{}, height{};
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
//    auto err = glGetError();
//    //GL_INVALID_ENUM
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);



//    const GLenum discards[] = {GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0};
//    //const GLenum discards[] = {GL_DEPTH_ATTACHMENT};
//    glInvalidateFramebuffer(GL_FRAMEBUFFER, 2, discards);


    //glFlush();

    return true;
}


void ManglMainWindow::onGlRealize(GtkGLArea* area) {

    gtk_gl_area_make_current(area);

    if (gtk_gl_area_get_error(area) != nullptr)
        return;


    auto result = glewInit();
    apl::verify(GLEW_OK == result || GLEW_ERROR_NO_GLX_DISPLAY == result, "GLEW Error: %s", (const char*)glewGetErrorString(result));

    /*
    //
    // Offscreen framebuffer
    //
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&oldFramebuffer_);
    //framebuffer_ = oldFramebuffer_;
    glGenFramebuffers(1, (GLuint *)&framebuffer_);  ManglAssertOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);   ManglAssertOpenGL();

    //
    // Color Renderbuffer
    //
    glGenRenderbuffers(1, (GLuint *)&colorRenderbuffer_);   ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);  ManglAssertOpenGL();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);

    auto screenSize = Env::applicationRect;


    GLint width{}, height{};
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    //
    // Depth Renderbuffer
    //
    glGenRenderbuffers(1, (GLuint *)&depthRenderbuffer_);  ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer_);   ManglAssertOpenGL();
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)screenSize.w, (GLsizei)screenSize.h);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)screenSize.w, (GLsizei)screenSize.h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer_);

    GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (GL_NO_ERROR != err) {
        auto s = (const char*)gluErrorString(err);
        MDebugFunc("%0X %s", (unsigned)err, s);
    }


    buffersAllocated_ = true;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);


    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
*/

    Framework::instance()->onInitialized();

    // Start timer
    auto fps = fps_;
    auto interval = 1000 / fps;
    g_timeout_add_full(G_PRIORITY_HIGH, interval, Self::timer_cb, this, nullptr);
}

void ManglMainWindow::onGlUnrealize(GtkWidget* widget) {

    gtk_gl_area_make_current(GTK_GL_AREA(widget));

    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != nullptr)
        return;

    if (buffersAllocated_) {
        buffersAllocated_ = false;
    }
}




//static gboolean onOpenGLRender(GtkGLArea *area, GdkGLContext *context, gpointer userData)
//{
//    {
//        auto fw_ = MFrameworkNative::instance();
//        auto renderer = fw_->getRenderer();
//
//        MFrameworkNative::TimerLockGuard lock{fw_->timerMutex_};
//        renderer->onRender();
//    }
//
////    glClearColor(1, 0, 0, 1);
////    glClear(GL_COLOR_BUFFER_BIT);
//
//    glFlush();
//    return TRUE;
//}
//
//static void onOpenGLRealize(GtkGLArea *area)
//{
//    gtk_gl_area_make_current (area);
//
//    if (gtk_gl_area_get_error(area) != NULL)
//        return;
//
////    auto err = glewInit();
////    apl::verify(GLEW_OK == err, "GLEW Error: %s", (const char*)glewGetErrorString(err));
//
//
//    /*
//    GError *internal_error = NULL;
//    init_buffer_objects (&error);
//    if (error != NULL)
//    {
//        gtk_gl_area_set_error (area, error);
//        g_error_free (error);
//        return;
//    }
//
//    init_shaders (&error);
//    if (error != NULL)
//    {
//        gtk_gl_area_set_error (area, error);
//        g_error_free (error);
//        return;
//    }
//    */
//}
//
//static void onOpenGLUnrealize (GtkWidget *gwidget)
//{
//    gtk_gl_area_make_current (GTK_GL_AREA (gwidget));
//
//    if (gtk_gl_area_get_error (GTK_GL_AREA (gwidget)) != NULL)
//        return;
//
////    glDeleteBuffers (1, &position_buffer);
////    glDeleteProgram (program);
//}



void ManglMainWindow::initOpenGL()
{
    // auto err = glewInit();
    // apl::verify(GLEW_OK == err, "GLEW Error: %s", (const char*)glewGetErrorString(err));

    // auto err = glewInit();
    // apl::verify(GLEW_OK == err || GLEW_ERROR_NO_GLX_DISPLAY == err, "GLEW Error: %s", (const char*)glewGetErrorString(err));

    /*
    GtkWidget *glArea = gtk_gl_area_new();

    //gtk_container_add(GTK_CONTAINER(gWindow), glArea);

    auto box = gtk_box_new (GTK_ORIENTATION_VERTICAL, FALSE);
    int boxMargin = 2;
    int marginTop = 0 ;
    int marginBottom = 16;
    gtk_widget_set_margin_start (box, boxMargin);
    gtk_widget_set_margin_end (box, boxMargin);
    gtk_widget_set_margin_top (box, marginTop);
    gtk_widget_set_margin_bottom (box, marginBottom);

    gtk_box_set_spacing (GTK_BOX (box), 0);
    gtk_window_set_child (GTK_WINDOW (gWindow), box);


    gtk_widget_set_hexpand (glArea, TRUE);
    gtk_widget_set_vexpand (glArea, TRUE);
    gtk_widget_set_size_request (glArea, 100, 200);  // TODO: size from window client area
    gtk_box_append (GTK_BOX (box), glArea);


    auto err = glewInit();
    apl::verify(GLEW_OK == err || GLEW_ERROR_NO_GLX_DISPLAY == err, "GLEW Error: %s", (const char*)glewGetErrorString(err));

    g_signal_connect(glArea, "render", G_CALLBACK(onOpenGLRender), this);
    g_signal_connect(glArea, "realize", G_CALLBACK(onOpenGLRealize), this);
    g_signal_connect(glArea, "unrealize", G_CALLBACK(onOpenGLUnrealize), this);
     */
}

void ManglMainWindow::releaseOpenGL()
{
}


}  // namespace mangl

#endif
