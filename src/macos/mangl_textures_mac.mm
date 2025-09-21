#include "../core/mangl_render.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

#include <apeal/file.hpp>

namespace mangl {

void SceneRenderer::loadTextureData(StrView filename, apl::Image &image)
{
    @autoreleasepool {
        auto contents = apl::loadFile(filename);

        NSData *data = [NSData dataWithBytes:contents.data() length:contents.length()];

        CGImageRef cgImage = [[NSBitmapImageRep imageRepWithData: data] CGImage];

        auto textureSize = apl::Size<Index>::make(CGImageGetWidth(cgImage), CGImageGetHeight(cgImage));

        image.create({.cm = apl::ColorModel::RGBA, .size = textureSize});
        
        uint32_t flags = kCGImageAlphaPremultipliedLast;
        flags |= kCGBitmapByteOrder32Big;

        auto colorSpace = CGColorSpaceCreateDeviceRGB();
        auto context = CGBitmapContextCreate(
                  image.buffer.data(),
                  textureSize.w, textureSize.h,
                  8,
                  4 * textureSize.w,
                  colorSpace,
                  flags);
        
        auto textureRect = CGRectMake(0, 0, textureSize.w, textureSize.h);
        
        CGColorSpaceRelease(colorSpace);
        CGContextClearRect(context, textureRect);
        CGContextDrawImage(context, textureRect, cgImage);

        CGContextRelease(context);
    }
}

} // namespace mangl


