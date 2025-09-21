/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_common.h"
#include "mangl_environment.h"

#include <apeal/file.hpp>

#ifdef __OBJC__
#include <Foundation/Foundation.h>

#if APEAL_OS_IPHONE
#include <UIKit/UIKit.h>
#endif
#endif

namespace mangl {

class Resources: public apl::Singleton<Resources> {

private:
    using SuffixList = StrValCont<String>;
    using Key = int;

    static KeyValCont<Key, String> resList_, bundlePathList_;
    static SuffixList suffixes_, backupSuffixes_, extSubstList_;

public:
    static StrView soundExt;
    static StrView musicExt;

    static StrView textureImageExt;
    static StrView textureAtlasExt;
    static String textureSuffix;

public:
    // Setup
    static void clearSuffixes() { suffixes_.clear(); }
    static void addSuffix(StrView ext, StrView suffix) { suffixes_[ext] = suffix; }
    static void setTextureSuffix(StrView suffix) noexcept { textureSuffix = suffix;};

    static void addBackupSuffix(StrView ext, StrView suffix) { backupSuffixes_[ext] = suffix; }

    static void addExtSubst(StrView ext, StrView subst) { extSubstList_[ext] = subst; }

public:

    template <class Id>
    static void addPath(Id resid, StrView path) {
        resList_[static_cast<Key>(resid)] = processPath(path);
    }

    template <class Id>
    static void addBundlePath(Id resid, StrView path) {
        bundlePathList_[static_cast<Key>(resid)] = processPath(path);
    }

    template <class Id>
    static void addBundleAtlasPath(Id texureId, StrView path) {
        addBundlePath(texureId, apl::makeFileName(path, textureAtlasExt));
    }


    template <class Id>
    static void addBundleTexturePath(Id texResid, Id atlasResid, StrView path) {
        addBundlePath(texResid, apl::makeFileName(path, textureImageExt));
        addBundlePath(atlasResid, apl::makeFileName(path, textureAtlasExt));
    }

    template <class Id>
    static void addBundleSoundPath(Id resid, StrView path) {
        addBundlePath(resid, apl::makeFileName(path, soundExt));
    }

    template <class Id>
    static void addBundleMusicPath(Id resid, StrView path) {
        addBundlePath(resid, apl::makeFileName(path, musicExt));
    }

    template <class Id>
    static void addURL(Id resid, StrView url) {
        addPath(resid, url);
    }


private:
    static String processSuffix(StrView path, SuffixList &suffixes) {
        auto extPos = path.find_last_of('.');

        auto ext = path.substr(extPos + 1);

        String finalPath{path};

        if (auto it = suffixes.find(ext); it != suffixes.end()) {
            finalPath.erase(extPos);
            finalPath += '-' + apl::makeFileName(it->second, ext);
        }

        extPos = finalPath.find_last_of('.');
        if (auto it = extSubstList_.find(ext); it != extSubstList_.end()) {
            finalPath.erase(extPos);
            finalPath += '.';
            finalPath += it->second;
        }
        return finalPath;
    }

    static String processPath(StrView path, bool tryBackupSuffix = false) {
        if (extSubstList_.empty() && suffixes_.empty())
            return String{path};

        if (auto pos = path.find_last_of('.'); pos == path.npos)
            return String{path};

        auto finalPath = processSuffix(path, suffixes_);

        if (tryBackupSuffix && !apl::fileExists(finalPath))
            finalPath = processSuffix(path, backupSuffixes_);

        return finalPath;
    }



public:  // access
    template <class IdT>
    static String getName(IdT resid) {
        auto r = resList_.find(static_cast<Key>(resid));

        if (r != resList_.end())
            return r->second;

        return {};
    }

    template <class IdT>
    static String path(IdT resid) {
        String s;

        auto r = resList_.find(static_cast<Key>(resid));

        if (r != resList_.end())
            return r->second;

        r = bundlePathList_.find(resid);

        if (r != bundlePathList_.end()) {

            // if the file starts with ':' then it's a resource file
            if (r->second[0] != ':') {
                if (bundleDir() != ".") {
                    s = bundleDir();
                    if (r->second[0] != '/')
                        s += '/';
                }
            }
            s += r->second;

            return s;
        }

        MDebugTrapAssert(!s.empty());

        return s;
    }

    template <class IdT>
    static String texturePath(IdT resId) {
        String s;

        auto r = bundlePathList_.find(static_cast<Key>(resId));

        if (r != bundlePathList_.end()) {

            // if the file starts with ':' then it's a resource file
            if (r->second[0] != ':') {
                if (bundleDir() != ".") {
                    s = bundleDir();
                    if (r->second[0] != '/')
                        s += '/';
                }
            }
            s += r->second;

            return s;
        }

        MDebugTrapAssert(!s.empty());

        return s;
    }

    template <class IdT>
    static auto url(IdT resid) { return path(resid); }

    static auto makeBundleMusicPath(StrView filename) noexcept {
        return makeBundlePathExt(filename, musicExt);
    }

    static auto makeBundleSoundPath(StrView filename) noexcept {
        return makeBundlePathExt(filename, soundExt);
    }


#if defined(__OBJC__)
    template <class IdT>
    static NSString *nsstring(IdT resid) {
        return[NSString stringWithUTF8String: path(static_cast<Key>(resid)).c_str()];
    }

#if APEAL_OS_IOS
    template <class IdT>
    static UIImage *uiimage(IdT resid) {
        NSString *p = nsstring(resid);
        UIImage *image = [UIImage imageWithContentsOfFile:p];

#if _DEBUG
        verify(image, "Cannot load image: %s", path(resid));
#endif
        return image;
    }
#endif
#endif  // if Objective C



public:
    static const String& bundleDir() noexcept {
        return Env::instance()->bundleDir;
    }

    static String bundleDirJoin(StrView path) noexcept {
        String joined;
        joined.reserve(bundleDir().size() + 1 + path.size());
        joined = bundleDir() + '/';
        joined += path;
        return joined;
    }

    static String makeBundlePathExt(StrView path, StrView ext) noexcept {
        return bundleDirJoin(apl::makeFileName(path, ext));
    }
};

using Res = Resources;

}  // namespace mangl

