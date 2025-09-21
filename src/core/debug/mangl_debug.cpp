/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_debug.h"

//#include <apeal/debug.cpp>

/*
namespace apl {
constexpr int32_t basename_index (const char * const path, const int32_t index = 0, const int32_t slash_index = -1)
{                                                                                                                                                             
   return path [index]                                                                                                                                    
       ? ( path [index] == '/'                                                                                                                            
           ? basename_index (path, index + 1, index)                                                                                                      
           : basename_index (path, index + 1, slash_index)                                                                                                
           )                                                                                                                                              
       : (slash_index + 1)                                                                                                                                
       ;                                                                                                                                                  
}                                                                                                                                                             
}


#define STRINGIZE_DETAIL(x) #x                                                                                                                                
#define STRINGIZE(x) STRINGIZE_DETAIL(x)                                                                                                                      


#define __FILELINE__ ({ static const int32_t basename_idx = apl::basename_index(__FILE__); \
                        static_assert (basename_idx >= 0, "compile-time basename");   \
                        __FILE__ ":" STRINGIZE(__LINE__) ": " + basename_idx; })
*/

