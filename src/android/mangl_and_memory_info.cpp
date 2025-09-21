/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../include/mangl/memory_info.h"

namespace mangl {

//namespace android {
//    unsigned long long android_total_memory, android_free_memory, android_used_memory;
//}

MemoryInfo::MemoryInfo()
{
	totalMemory = freeMemory = usedMemory = 0 ;
}


void MemoryInfo::load()
{
    // TODO: memory
	totalMemory = 0; //android::android_total_memory;
	freeMemory = 0; //android::android_free_memory;
	usedMemory = 0; //android::android_used_memory;
}

}  // namespace mangl
