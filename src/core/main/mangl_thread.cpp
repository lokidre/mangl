/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_thread.h"

#include "../mangl_types.h"
#include "../mangl_util.h"

#include "../mangl_framework.h"

namespace mangl {

namespace M {

ThreadId getCurrentThreadId() noexcept {
    
 uint64_t threadId{};
 
#if APEAL_OS_WINDOWS
    threadId = GetCurrentThreadId();
#elif APEAL_OS_MAC
    pthread_threadid_np(nullptr, &threadId);
#else
    threadId = gettid();
#endif

    return threadId;
}

//const MString emptyString ;  // dummy empty string

//apl::ACrc8 crc8;
}

}


/*
void mangl_init(MFrameworkNative *framework)
{
	mangl_framework_ = framework ;
}
*/

