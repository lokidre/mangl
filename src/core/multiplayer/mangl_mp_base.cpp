/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_base.h"
#include "../mangl_framework.h"

#include <chrono>

namespace mangl::mmp {

uint64_t MpGameBase::currentPacketIndex_{1};


MpGameBase::~MpGameBase() {
    clearCallbacks();
}



void MpGameBase::setupMpGameBase(const MpGameParams& params) {
    params_ = params;

    if (params_.dataTransferFormat == net::DataTransferFormat::Unknown) {
        params_.dataTransferFormat = net::DataTransferFormat::Packet;
    }

    if (params_.debugVerboseAll) {
        params_.debugVerboseConState = true;
        params_.debugVerbosePacket = true;
        params_.debugVerboseMatch = true;
    }

    if (!params_.debugPacketLogDir.empty())
        params_.debugPacketLogEnabled = true;
}


void MpGameBase::startMpGameBase() {
    started_ = true;
    lastTimestamp_ = currentTimestamp_ = Framework::timestamp();
    
    timerHandler_.registerHandler(std::bind(&MpGameBase::baseProcessTimer, this, std::placeholders::_1));
}

void MpGameBase::shutdownMpGameBase() {
    started_ = false;
    finished_ = true;
    timerHandler_.unregister();
}

void MpGameBase::baseProcessTimer(Timestamp timestamp) {
    currentTimestamp_ = timestamp;
    currentEpochTimeUpdated_ = false;

    onBaseTimer(timestamp);


    lastTimestamp_ = currentTimestamp_;
}




uint64_t MpGameBase::updateCurrentEpochTime() noexcept {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = duration_cast<milliseconds>(now.time_since_epoch()).count();

	currentEpochTime_ = time;

	currentEpochTimeUpdated_ = true;

    return currentEpochTime_;
}


}  // namespace mangl::mmp


