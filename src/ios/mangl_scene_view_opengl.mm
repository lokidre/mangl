#include "mangl_scene_view_opengl.h"


#if MANGL_OPENGL

#include "../core/mangl_framework.h"
#include "../core/mangl_render.h"

@import QuartzCore;

#include "../core/opengl/mangl_render_opengl.h"

using namespace mangl;

static ManglSceneViewOpenGL* manglMainSceneViewOpenGL__{};

@implementation ManglSceneViewOpenGL

+ (ManglSceneViewOpenGL *)instance
{
    return manglMainSceneViewOpenGL__;
}

- (void)ctor
{
    render_ = nullptr;
    transparent = false;
    buffersAllocated_ = false;
}

+ (Class)layerClass {  return [CAEAGLLayer class]; }

- (id)initWithFrame:(CGRect)rect
{
    self = [super initWithFrame:rect];
    manglMainSceneViewOpenGL__ = self;
    [self ctor];
    return self;
}

- (void)dealloc
{
    MDebugFunc();

    [self dtor];
}

- (void)dtor
{
    if (nil != context_) {

        if (EAGLContext.currentContext == context_) {
            [EAGLContext setCurrentContext:nil];
        }
        context_ = nil;
    }

    if (buffersAllocated_) {

        glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffer_);
        glDeleteFramebuffers(1, &framebuffer_);

        glDeleteRenderbuffers(1, &colorRenderbuffer_);
        glDeleteRenderbuffers(1, &depthRenderbuffer_);

        buffersAllocated_ = false;
    }
}


- (void)attachRender:(SceneRenderer *)render
{
    render_ = render;
    render_->transparent = transparent;

    auto eaglLayer = (CAEAGLLayer *)self.layer;

    if (transparent) {
        self.backgroundColor = UIColor.clearColor;
        eaglLayer.opaque = false;
    } else {
        eaglLayer.opaque = true;
    }


//    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
//                  [NSNumber numberWithBool:NO],  kEAGLDrawablePropertyRetainedBacking,
//                  kEAGLColorFormatRGBA8,  kEAGLDrawablePropertyColorFormat,
//                  // kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
//                  nil];
    
    eaglLayer.drawableProperties = @{
        kEAGLDrawablePropertyRetainedBacking: @(NO),
        kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8,
    };
                  // kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
//                  nil];


    context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    if (!context_) {
        // Fall back from ES3 to ES2
        context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

        if (!context_)
            @throw([NSException exceptionWithName:@"Error" reason:@"Cannot create OpenGL context" userInfo:nil]);
    }


    oldContext_ = EAGLContext.currentContext;

    if (![EAGLContext setCurrentContext:context_])
        @throw([NSException exceptionWithName:@"Error" reason:@"Cannot set OpenGL context" userInfo:nil]);



    //
    // Offscreen framebuffer
    //
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&oldFramebuffer_);
    glGenFramebuffers(1, &framebuffer_);  ManglAssertOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);   ManglAssertOpenGL();


    //
    // Color Renderbuffer
    //
    glGenRenderbuffers(1, &colorRenderbuffer_);   ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);  ManglAssertOpenGL();
    [context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);


    GLint width{}, height{};
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    //
    // Depth Renderbuffer
    //
    glGenRenderbuffers(1, &depthRenderbuffer_);  ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer_);   ManglAssertOpenGL();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer_);



    buffersAllocated_ = true;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        DLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);

    MFrameworkNative::instance()->onInitialized();
}

- (void)renderScene
{
    //glDepthMask(GL_TRUE);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    render_->onRender();

    const GLenum discards[] = {GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0};
    //const GLenum discards[] = {GL_DEPTH_ATTACHMENT};
    glInvalidateFramebuffer(GL_FRAMEBUFFER, 2, discards);
    
    //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    //glInvalidateFramebuffer(GL_FRAMEBUFFER, 2, discards);
    //glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, discards);


    //glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
    [context_ presentRenderbuffer:GL_RENDERBUFFER];

    //const GLenum colorDiscards[] = {GL_COLOR_ATTACHMENT0};
    //glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, colorDiscards);
    //glInvalidateFramebuffer(GL_FRAMEBUFFER, 2, discards);

}

@end

#endif
