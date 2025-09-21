/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../../include/mangl/memory_info.h"

#include <apeal/def.hpp>

#if APEAL_OS_MAC

#include <mach/mach.h>
#include <mach/mach_host.h>

namespace mangl {

MemoryInfo::MemoryInfo()
{
}


void MemoryInfo::load()
{
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;

    host_port = mach_host_self() ;
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize) ;

    vm_statistics_data_t vm_stat;

    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS)
        return;  // failure

    natural_t mem_used = natural_t((vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize);
    natural_t mem_free = natural_t(vm_stat.free_count * pagesize) ;
    natural_t mem_total = mem_used + mem_free;

    freeMemory = mem_free;
    totalMemory = mem_total;
    usedMemory = mem_used;

    //NSLog(@"used: %u free: %u total: %u", mem_used, mem_free, mem_total);
}

}  // namespace mangl

#endif

