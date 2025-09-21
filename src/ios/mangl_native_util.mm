#import "../apple/mangl_imacos_util.h"
#include "../core/mangl_error.h"

@import UIKit;


//
// MARK: Web
//
#include "../core/native/mangl_web.h"

namespace mangl {

bool Web::openUrl(StrView url)
{
    auto app = UIApplication.sharedApplication;
    auto urlo = [NSURL URLWithString:toNSString(url)];
    bool result{};

    [app openURL:urlo options:@{} completionHandler:^(BOOL success) {
    }];
    
    return result;
}
}   // namespace mangl


//
// MARK: MAppStore
//
#include "../core/native/mangl_appstore.h"

namespace mangl {

void AppStore::openAppHome(StrView appId)
{
    auto country = fromNSString([NSLocale.currentLocale objectForKey:NSLocaleCountryCode]);
    auto urlFmt = "itms-apps://itunes.apple.com/%s/app/id%s";
    auto urls = apl::fmt(urlFmt, country, appId);
    
    //auto fmt = @"itms-apps://itunes.apple.com/%@/app/id%@";
    //NSString* country = [NSLocale.currentLocale objectForKey:NSLocaleCountryCode];
    //auto urls = [NSString stringWithFormat:fmt, country, toNSString(appId), nil];
    auto url = [NSURL URLWithString:toNSString(urls)];
    auto app = UIApplication.sharedApplication;
    
    [app openURL:url options:@{} completionHandler:^(BOOL success) {
        if (!success)
            mthrowError("Cannot open app home: %s", appId);
    }];

}

}   // namespace mangl
