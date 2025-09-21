/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"

#include "../core/mangl_color.h"


#ifdef __OBJC__

#if APEAL_OS_MAC
@import CoreFoundation;
#endif


#if APEAL_OS_IPHONE
@import UIKit;
#else
@import Cocoa;
#endif


#if ! __has_feature(objc_arc)
#error "ARC is disabled"
#endif


// Supporting formatting of NSString
template <class Str, class F>
void str_format_type(Str &str, F *fmt, F fend, NSString *ns) {
    *fmt = apl::str_format_collect_spec(*fmt, fend);
    str += ns.UTF8String;
}

// Support NSError formatting
template <class Str, class F>
void str_format_type(Str &str, F *fmt, F fend, NSError *err) {
    *fmt = apl::str_format_collect_spec(*fmt, fend) ;
    str += err.localizedDescription.UTF8String;
}

using MRect = mangl::Rect;
using MRectA = mangl::RectA;
using MString = mangl::String;
using MStrView = mangl::StrView;
using MCord = mangl::Cord;
using MSize = mangl::Size;

namespace mangl {

inline auto toCGRect(RectA r) noexcept {
    return CGRectMake(r.x, r.y, r.w, r.h);
}

inline auto fromCGRect(const CGRect& r) noexcept {
    return MRect{r.origin.x, r.origin.y, r.size.width, r.size.height};
}

inline auto toCGSize(SizeA s) noexcept {
    return CGSizeMake(s.w, s.h);
}

inline auto fromCGSize(const CGSize& s) noexcept {
    return MSize{s.width, s.height};
}

inline auto toCGPoint(PointA s) noexcept {
    return CGPointMake(s.x, s.y);
}

inline auto fromCGPoint(const CGPoint& p) noexcept {
    return MPoint{p.x, p.y};
}



#if defined(TARGET_OS_MAC_DESKTOP)
inline auto toNSRect(RectA r) noexcept {
    return NSMakeRect(r.x, r.y, r.w, r.h);
}

inline auto fromNSRect(const NSRect& r) noexcept {
    return MRect{r.origin.x, r.origin.y, r.size.width, r.size.height};
}

inline auto toNSSize(SizeA s) noexcept {
    return NSMakeSize(s.w, s.h);
}

inline auto fromNSSize(const NSSize& s) noexcept {
    return MSize{s.width, s.height};
}

inline auto toNSPoint(PointA s) noexcept {
    return NSMakePoint(s.x, s.y);
}

inline auto fromNSPoint(const NSPoint& p) noexcept {
    return MPoint{p.x, p.y};
}

#endif



inline String fromNSString(NSString *ns) {
    return ns ? ns.UTF8String : String{};
}


inline StrView toStringView(NSString *ns) noexcept {
    return ns ? ns.UTF8String : StrView{};
}

inline String toString(NSString *ns) noexcept {
    return ns ? ns.UTF8String : String{};
}


//inline bool toString ( NSString *ns, MString &s ) {
//	if ( nil == ns ) {
//		s.clear() ;
//		return false ;
//	}
//	s = ns.UTF8String ;
//	return true ;
//}

//inline NSString *toNSString(const char *str) {
//    return [NSString stringWithUTF8String: str] ;
//}


inline NSString *toNSString(StrView str) noexcept {
    return [[NSString alloc] initWithBytes:str.data() length:str.size() encoding:NSUTF8StringEncoding];
}

//inline NSString *toNSString(MStringA str) {
//    return [NSString stringWithUTF8String: str.c_str()] ;
//}



#if APEAL_OS_IPHONE

inline UIColor *toUIColor(ColorA c) noexcept {
	return [UIColor colorWithRed:c.r green:c.g blue:c.b alpha:c.a] ;
}

#else

inline NSColor *toNSColor(ColorA c) noexcept {
	return [NSColor colorWithRed:c.r green:c.g blue:c.b alpha:c.a] ;
}


#endif


// Objective C immitation of dynamic_cast
template<typename T>
T* objc_cast(id from) {
    if ([from isKindOfClass:[T class]]) {
        return static_cast<T*>(from);
    }
    return nil;
}

}  // namespace mangl

#endif  // ObjC


