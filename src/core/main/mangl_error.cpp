/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
//#include "../mangl_error.h"

//#include <string.h>
//#include <stdlib.h>

//#include <cstdio>
//#include <string>
//#include <stdexcept>

/*
void manglRaiseErrorBody ( const char * const file, int line, const char *const fmt, ... )
{
	char cs[4096] ;
	
	va_list ap ;
	va_start ( ap, fmt ) ;
#if __CC == __CC_MICROSOFTC
	_vsnprintf_s ( cs, sizeof(cs), _TRUNCATE, fmt, ap ) ;
#else
	vsnprintf ( cs, sizeof(cs), fmt, ap ) ;
#endif
	va_end(ap) ;

	char msg[4096] ;
#if __CC == __CC_MICROSOFTC
	_snprintf_s ( msg, sizeof(msg), _TRUNCATE, "%s\n\n%s:%d\n", cs, file, line ) ;
#else
	snprintf ( msg, sizeof(msg), "%s\n\n%s:%d\n", cs, file, line ) ;
#endif
	
	throw std::runtime_error(msg) ;
}


*/


