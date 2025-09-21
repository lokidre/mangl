/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "daemonize.h"

#if __unix__ || __linux__ || __posix

#include <csignal>
#include <unistd.h>
#include <cerrno>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <syslog.h>

#include <systemd/sd-daemon.h>

#include <cstdio>

#include <cstdlib>
#include <stdexcept>

// signal handler
void daemon_signal_handler(int sig)
{
    if (sig == SIGHUP) {  // hangup signal
    }


    if (sig == SIGTERM) {  // terminate signal
        sd_notify(0, "STOPPING=1");
        exit(daemon_exit_handler());
    }
}



// daemonize routine
// returns 0 if success, or some error code
extern "C"
int daemonize(const char *name)
{
    // check if we are already a daemon
    //if ( getppid() == 1 )
    //  throw std::runtime_error("already a daemon") ;


    // fork from the parent process
    pid_t pid = fork();

    // error occured
    if (pid < 0)
        throw std::runtime_error("fork() error");

    // exit parent process
    if (pid > 0 )
        return pid;


    // if we are allowed to set sid
    pid = setsid();

    if (pid == -1)
        throw std::runtime_error("setsid() error");

    // fork the second time
    pid = fork();

    // error occured
    if (pid < 0)
        throw std::runtime_error("fork() error");

    // exit parent process
    if (pid > 0)
        return pid;


    // close all console descriptors, and redirect them to "/dev/null"
    for (int x = sysconf(_SC_OPEN_MAX); x >=0 ; x--)
        close(x);

    //close(0), close(1), close(2);
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, 0);
    dup2(devnull, 1);
    dup2(devnull, 2);

    umask(0);  // optional
    chdir("/");  // optional


    signal(SIGCHLD, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGHUP, daemon_signal_handler);
    signal(SIGTERM, daemon_signal_handler);

    openlog(name, LOG_PID, LOG_DAEMON);

    fprintf(stderr, SD_NOTICE "Hello World!\n");

    sd_notify(0, "READY=1");
    //sd_notify(

    return 0;
}

#endif
