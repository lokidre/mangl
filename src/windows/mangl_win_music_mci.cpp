/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_music_mci.h"

#include "../core/audio/mangl_music.h"
#include "../core/mangl_dispatch.h"

#if MANGL_MUSIC_MCI

#if MANGL_SDL
#include "../sdl/mangl_sdl_main_window.h"
#else
#include "mangl_win_main_window.h"
#endif

#include <mmsystem.h>
#include <tchar.h>

namespace mangl {

void MusicEngineMCI::initialize() {
    ManglMainWindow::instance()->mciDevice_ = this;
}

void MusicEngineMCI::shutdown() {
    ManglMainWindow::instance()->mciDevice_ = nullptr;
}


bool MusicEngineMCI::mciCmd(UINT uMsg, DWORD_PTR dwParam1, void* dwParam2) {
    if (!mciDeviceId_)
        return false;

    auto result = mciSendCommand(mciDeviceId_, uMsg, dwParam1, (DWORD_PTR)dwParam2);
    return mciCheckResult(result);
}


void MusicEngineMCI::mciCmdPlay(StrView filename) {

    // Start playing
    apl::ApiString fn{filename};

    MCI_OPEN_PARMS op{};
    op.lpstrDeviceType = (LPCWSTR)(unsigned long long)MCI_ALL_DEVICE_ID;
    op.lpstrElementName = fn.win();
    op.lpstrAlias = L"song1";

    auto result = mciSendCommand(mciDeviceId_, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD_PTR)&op);

    if (!mciCheckResult(result))
        return;

    //auto result = mciCmd(MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, &op);
    if (!op.wDeviceID)
        return;

    mciDeviceId_ = op.wDeviceID;

    MCI_PLAY_PARMS pp{};

#if MANGL_WINMAIN
    pp.dwCallback = (DWORD_PTR)ManglMainWindow::instance()->hwnd();
#endif

    mciCmd(MCI_PLAY, MCI_NOTIFY, &pp);


}


void MusicEngineMCI::mciCmdClose() {
    MCI_GENERIC_PARMS gp{};
    mciCmd(MCI_CLOSE, MCI_WAIT, &gp);
    mciDeviceId_ = 0;
}

void MusicEngineMCI::mciCmdStop() {
    MCI_GENERIC_PARMS gp{};

    mciCmd(MCI_STOP, MCI_WAIT, &gp);
    //mciCmd(MCI_CLOSE, MCI_WAIT, &gp);

    //mciDeviceId_ = 0;
};


bool MusicEngineMCI::mciCheckResult(int code) {
    if (code == 0)
        return true;

    constexpr UINT len = 256;
    TCHAR text[len + 1];
    if (!mciGetErrorString(code, text, len)) {
        MDebugFunc("Unknown!");
        return false;
    }

    MDebugFunc(apl::apiFromWin(text));
    return false;
}

void MusicEngineMCI::onNotify() {
    scheduleClose_ = true;
    //{
    //    UniqueLock lock{mciMutex_};

    //    commandQueue_ += {Command::CLOSE};

    //    cmdReady_.notify_one();
    //}


    //Dispatch::asyncMain([] {
    //    Music::trackFinishedTrigger = true; // Notify about the track
    //});
}


void MusicEngineMCI::setVolume(Real volume, Real ramp) {
    //MCI_GENERIC_PARMS gp{};
    //mciCmd(MCI_SETAUDIO, MCI_WAIT, &gp);

    //UniqueLock lock{mciMutex_};

    //commandQueue_ += {Command::VOLUME, volume, ramp};

    //cmdReady_.notify_one();
}


void MusicEngineMCI::play(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Real volume) 
{
    // Stop if something is playing already
    if (mciDeviceId_) {
        mciCmdStop();
        mciCmdClose();
    }

    currentTrackId_ = track->trackId;
    mciCmdPlay(track->filename);

    Music::processTrackStart(currentTrackId_);
}


void MusicEngineMCI::playCrossfade(MusicTrackContextP track, Timestamp delay, Timestamp ramp, Timestamp gap, Real volume) {
    // just call play for now
    play(track, delay, ramp, volume);
}



void MusicEngineMCI::stop(Timestamp when, Timestamp ramp) {
    mciCmdStop();
    //currentTrack_ = {};
}

bool MusicEngineMCI::playing() {
    return mciDeviceId_ == 0 ? false : true;

    /*

    MCIERROR mci_error ;

    //UNICODE

    TCHAR text[256] ;
    UINT len = 256 ;
    mci_error = mciSendString(_T("status song1 mode"), text, len, 0 ) ;

    {
        text[7] = len ;
        if ( _tcscmp(text, _T("playing")) == 0 )
            return true ;
    }


    mci_device_open_ = false ;
    */
}

void MusicEngineMCI::pause() {
    MCI_GENERIC_PARMS gp{};
    mciCmd(MCI_PAUSE, MCI_WAIT, &gp);
}

void MusicEngineMCI::resume() {
    MCI_GENERIC_PARMS gp{};
    mciCmd(MCI_RESUME, MCI_WAIT, &gp);
}


void MusicEngineMCI::processTimer(Timestamp) {
    if (scheduleClose_) {
        scheduleClose_ = false;
        mciCmdClose();
    }
}


void MusicEngineMCI::onWndMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (MM_MCINOTIFY == msg) {
        if (MCI_NOTIFY_SUCCESSFUL == wParam) {
            Music::processTrackFinish(currentTrackId_);
        }
    }
}



} // namespace mangl


#endif
