#import <AppKit/AppKit.h>

#import "../apple/mangl_imacos_util.h"

//
// MARK: Web
//
#include "../core/native/mangl_web.h"

namespace mangl {

bool Web::openUrl(StrView url)
{
    auto nsurl = [NSURL URLWithString:toNSString(url)];
    [NSWorkspace.sharedWorkspace openURL:nsurl];
    return true;
}

} // namespace mangl


//
// MARK: AppStore
//
#include "../core/native/mangl_appstore.h"

namespace mangl {

void AppStore::openAppHome(StrView appId)
{
    auto link = apl::fmt("macappstore://itunes.apple.com/app/id%s", appId);

    //NSString *fmt = @"macappstore://itunes.apple.com/app/id%@" ;
    //NSString *url = [NSString stringWithFormat:fmt, toNSString(appId)] ;

    auto nsurl = [NSURL URLWithString:toNSString(link)];
    [NSWorkspace.sharedWorkspace openURL:nsurl];
}

}  // namespace mangl

