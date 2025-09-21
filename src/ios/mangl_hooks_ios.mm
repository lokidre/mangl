#import "mangl_hooks_ios.h"

#include "../core/mangl_common.h"

@implementation ManglIosHooks

- (id)init {
    self = [super init];
    self.hooks = nil;
    return self;
}

+ (ManglIosHooks *)instance {
    static ManglIosHooks *instance_ = nil;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        instance_ = [[self alloc] init];
    });
    return instance_;
}

- (void)registerHook:(id<ManglIosMainHook>)hook {
    if (self.hooks == nil)
        self.hooks = [[NSMutableArray<id<ManglIosMainHook>> alloc] init];

    [self.hooks addObject:hook];
}
@end


