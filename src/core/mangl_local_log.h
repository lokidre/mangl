/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/

/*
	Logging per file.

	Usage:
		In order to logging per file on do in inside the source file:

			#define MANGL_LOCAL_LOG 1
			#include "mangl_local_log.h"

		To turn the logging for that file off undefine the MANGL_LOCAL_LOG

*/

#if MANGL_LOCAL_LOG

#define mlog(...) mdebug_print(__VA_ARGS__)

#define mlogf(...) mdebug_print(__VA_ARGS__)

//#if _MSC_VER
//#define mlogf(__fmt,...) mdebug_printf(__fmt,__VA_ARGS__)
//#else
//#define mlogf(__fmt,__args...) mdebug_printf(__fmt,##__args)
//#endif

#else

#define mlog(...) void(0)
#define mlogf(...) void(0)

//#if _MSC_VER
//#define mlogf(__fmt,...) void(0)
//#else
//#define mlogf(__fmt,__args...) void(0)
//#endif

#endif
