#include "../core/mangl_storage.h"
#include "../core/mangl_util.h"

#include "mangl_imacos_util.h"

namespace mangl {


String StorageEngineNSUserDefaults::readString(StrView key)
{
    auto defs = NSUserDefaults.standardUserDefaults;
    auto nsname = toNSString(key);

    if ([defs objectForKey:nsname] == nil)
        return String{};

    return toString([defs stringForKey:nsname]);
}

void StorageEngineNSUserDefaults::writeString(StrView key, StrView value)
{
    auto defs = NSUserDefaults.standardUserDefaults;
    [defs setObject:toNSString(value) forKey:toNSString(key)];
    //[defs synchronize];
}

}   // namespace mangl

