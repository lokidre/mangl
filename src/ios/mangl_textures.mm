#include "../core/mangl_render.h"

#include "../core/mangl_types.h"
#include "../apple/mangl_imacos_util.h"

#include <apeal/file.hpp>

namespace mangl {

void SceneRenderer::loadTextureData(StrView filename, apl::Image* image)
{
    @autoreleasepool {
        UIImage *uiImage = [[UIImage alloc] initWithContentsOfFile:toNSString(filename)];

        apl::verify(uiImage, "Unable to load: %s", apl::extractFileName(filename));

        auto textureSize = apl::Size<int>::make(CGImageGetWidth(uiImage.CGImage), CGImageGetHeight(uiImage.CGImage));

        image->create({.cm = apl::ColorModel::RGBA, .size = textureSize});

        auto colorSpace = CGColorSpaceCreateDeviceRGB();
        auto context = CGBitmapContextCreate(image->buffer.data(),
                                             textureSize.w,
                                             textureSize.h,
                                             8,
                                             4 * textureSize.w,
                                             colorSpace,
                                             kCGImageAlphaPremultipliedLast);
        CGColorSpaceRelease(colorSpace);
        CGContextClearRect(context, CGRectMake(0, 0, textureSize.w, textureSize.h));
        CGContextTranslateCTM(context, 0, textureSize.h - textureSize.h);
        CGContextDrawImage(context, CGRectMake(0, 0, textureSize.w, textureSize.h), uiImage.CGImage);

        CGContextRelease(context);
    }
}

}  // namespace mangl

