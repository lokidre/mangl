/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_jni.h"

#include "../core/social/mangl_invites.h"

namespace mangl {

static jmethodID jmInviteSMS{};
static jmethodID jmInviteEmail{};

void Jni::registerInvitesMethods() {
    activityMethods_.insert(activityMethods_.end(), {
        {&jmInviteSMS, "jmInviteSMS", "(Ljava/lang/String;)V"},
        {&jmInviteEmail, "jmInviteEmail", "(Ljava/lang/String;Ljava/lang/String;)V"},
    });
}

void Invites::inviteSMS(InviteParamsA params) {
    JniString message{params.message};
    Jni::callActivityVoidMethod(jmInviteSMS, message.jstr);
}

void Invites::inviteEmail(InviteParamsA params) {
    JniString message{params.message};
    JniString title{params.title};
    Jni::callActivityVoidMethod(jmInviteEmail, message.jstr, title.jstr);
}

} // namespace mangl
