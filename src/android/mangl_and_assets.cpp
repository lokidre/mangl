/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_assets.h"
#include "mangl_and_main_engine.h"


namespace mangl {

MAssetFileAndroid::~MAssetFileAndroid() {
    close();
}

void MAssetFileAndroid::open(StrView filename) {
    close();

    auto name = filename[0] == ':' ? filename.substr(filename.find_first_of('/') + 1) : filename;

    apl::ApiString apiName{name};

    auto assetManager = Jni::app->activity->assetManager;

    asset_ = AAssetManager_open(assetManager, apiName.c_str(), AASSET_MODE_RANDOM);

    apl::verify(asset_, "Cannot open asset: %s", filename);

//    if (!asset_)
//        throw std::runtime_error("Can't open asset: " + filename);
}

void MAssetFileAndroid::close() {
    if (asset_) {
        AAsset_close(asset_);
        asset_ = nullptr;
    }
}

int MAssetFileAndroid::read(void *buffer, int count) {
    return AAsset_read(asset_, buffer, count);
}

int MAssetFileAndroid::size() {
    return (int) AAsset_getLength(asset_);
}

} // namespace mangl

using namespace mangl;

int android_assets_stat(const char *filename, struct stat *st) {
    //logi ( "stat: %s", filename ) ;

    String name = filename;
    if (name[0] == ':')
        name = name.substr(name.find_first_of('/') + 1);

    auto assetManager = Jni::getActivity()->assetManager;

    AAssetDir *assetDir = AAssetManager_openDir(assetManager, "");

    if (!assetDir)
        Jni::die("Cannot open assets dir");

    for (;;) {
        const char *first = AAssetDir_getNextFileName(assetDir);
        if (!first) {
            logi("  not found file");
            break;
        }

        //debug_printf ( "  iter: %s == %s", first, name.c_str() ) ;

        if (name == first) {
            AAssetDir_close(assetDir);
            return 0;
        }
    }
    AAssetDir_close(assetDir);

    return -1;
}


int AndroidAssetManager::openFile(const std::string& filename) {
    //using namespace android;

/*
	AAssetDir* xpCurrentDir = AAssetManager_openDir( assetManager_, "" );
	for(;;) {
		const char *first = AAssetDir_getNextFileName(xpCurrentDir);
		if ( !first )
			break ;
		debug_printf("iter: %s", first);

		//asset_ = AAssetManager_open ( assetManager_, first, AASSET_MODE_BUFFER  ) ;

		//if ( asset_ == NULL )
		//	throw std::runtime_error(std::string("Assets::Open error: ") + first) ;
	}
	AAssetDir_close(xpCurrentDir) ;
*/

    // Skip the first directory sign

    std::string name = filename[0] == ':' ? filename.substr(filename.find_first_of('/') + 1) : filename;

    //MDebugPrint("Asset: %s", name);

    auto assetManager = Jni::getActivity()->assetManager;

    asset_ = AAssetManager_open(assetManager, name.c_str(), AASSET_MODE_RANDOM);

    if (!asset_)
        throw std::runtime_error("Assets::Open error: " + name);

    return 1;
}

void AndroidAssetManager::closeFile() {
    if (!asset_)
        return;

    AAsset_close(asset_);
    asset_ = 0;
}

int64_t AndroidAssetManager::readFile(void *buffer, int64_t size) {
    int64_t remaining = size;
    uint8_t *ptr = (uint8_t *) buffer;

    while (remaining > 0) {
        int count = AAsset_read(asset_, ptr, size_t(remaining));

        //debug_printf ( "read count: %d", count ) ;

        if (count < 0)
            throw std::runtime_error("Assets:Error reading asset");

        ptr += count;
        remaining -= count;
    }

    return size - remaining;
}

int64_t AndroidAssetManager::writeFile(const void *buffer, int64_t count) {
    return -1;  // write is not supported
}

int64_t AndroidAssetManager::seek(int64_t position, int origin) {
    //logi ( "assets::seek::%d, %d", (int)position, origin ) ;

    int64_t result = AAsset_seek64(asset_, position, origin);

    if (result < 0)
        throw std::runtime_error("Assets:Error seeking file position");

    return result;
}

int64_t AndroidAssetManager::getSize() {
    int64_t size = AAsset_getLength64(asset_);

    //logi ( "assets::seek::%d", (int)size ) ;

    return size;
}

//}  // namespace mangl
