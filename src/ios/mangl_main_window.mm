#import "mangl_main_window.h"

#include "../core/mangl_environment.h"
#include "../core/mangl_debug.h"

using namespace mangl;

@implementation ManglMainWindow

- (id)initWithFrame:(CGRect)rect
{

    @try {
        self = [super initWithFrame:rect];

        self.mainViewController = [[ManglMainViewController alloc] initWithViewController:0];
        [self setRootViewController:self.mainViewController];
        
        
        
        //
        // Screen resolution
        //
        auto screen = self.screen;
//        auto mainScreen = UIScreen.mainScreen;
        auto mainScreen = screen;
        
        auto nativeBounds = mainScreen.nativeBounds.size;
        auto nativeScale = mainScreen.nativeScale;
        
        auto logicalScreen = mainScreen.bounds.size;
        auto logicalScale = mainScreen.scale;

        Env::physScreenSize = {nativeBounds.width, nativeBounds.height};
        //UIScreen.mainScreen.displa

        //screenSize = {physScreen.width * physScale, physScreen.height * physScale};
        //screenSize = physScreenSize;
        //applicationFrame = {.w = logicalScreen.width, .h = logicalScreen.height};
        Env::applicationRect.setSize(Env::physScreenSize);
        Env::layoutSize = {logicalScreen.width, logicalScreen.height};

    //    pixelDensity = logicalScale;
        Env::pixelDensity = nativeScale;
        Env::logicalScreenSize = {.w = logicalScreen.width, .h = logicalScreen.height};

        MDebugPrint("Screen: phys: %g, logical: %g", Env::physScreenSize, Env::logicalScreenSize);


        
        //screen.displayCornerRadius;
        
        auto scale = Env::pixelDensity;


        if (self.windowScene.statusBarManager)
            Env::statusBarHeight = self.windowScene.statusBarManager.statusBarFrame.size.height;


//        Env::statusBarHeight = UIApplication.sharedApplication.statusBarFrame.size.height;

        auto bottom = self.safeAreaInsets.bottom;
        auto top = self.safeAreaInsets.top;
        auto left = self.safeAreaInsets.left;
        auto right = self.safeAreaInsets.right;

        

        Env::screenPadding = {};
        Env::screenPadding.b = bottom * scale;
        Env::screenPadding.t = top * scale;
        Env::screenPadding.l = left * scale;
        Env::screenPadding.r = right * scale;
        
        // If the bottom is zero, then assume that's it's the rectangular screen
        if (Env::screenPadding.b == 0)
            Env::screenPadding.t = 0;
        
        // iPads report only bottom rounded corners
        if (Env::screenPadding.b != 0 && Env::screenPadding.t == 0)
            Env::screenPadding.t = Env::screenPadding.b;
    }
    @catch (NSException *e) {
        MDebugFunc("%s: %s", e.name, e.reason);
        return nil;
    }

    return self;
}


@end
