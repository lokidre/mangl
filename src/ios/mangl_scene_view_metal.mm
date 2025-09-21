#import "mangl_scene_view_metal.h"


#if MANGL_METAL

#include "../core/mangl_framework.h"

static ManglSceneViewMetal *manglMainSceneViewMetal__{};


@implementation ManglSceneViewMetal {
}

+ (ManglSceneViewMetal *)instance { return manglMainSceneViewMetal__; }


- (id)initWithFrame:(CGRect)rect {
    self = [super initWithFrame:rect];
    manglMainSceneViewMetal__ = self;
    return self;
}

@end

#endif


