/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"
#include "mangl_win_keyboard.h"

#if MANGL_WINMAIN

#include "../core/audio/mangl_music.h"

#if MANGL_MUSIC_MCI
#include "mangl_win_music_mci.h"
#endif

#include <mmsystem.h>
#include <uxtheme.h>


namespace mangl {

void Framework::finish() {
    auto mainWindow = ManglMainWindow::instance();

    PostMessage(mainWindow->hwnd(), WM_CLOSE, 0, 0);
}

static TouchInfo lastTouches_;

static inline Point convertLParamToPoint(LPARAM lParam) {
    auto p = Point::make(LOWORD(lParam), HIWORD(lParam));
    p /= Env::screenScale;
    p.y = Env::layoutSize.h - 1 - p.y;
    return p;
}

static inline TouchInfo getMouseTouchInfo(LPARAM lParam) {
    return {
        .p = convertLParamToPoint(lParam),
        .timestamp = Framework::currentTimestamp(),
        .index = 0,
        .count = 1,
    };
}

static inline TouchesInfo getMouseTouchesInfo(LPARAM lParam) {
    TouchesInfo touch{
        .count = 1,
        .touches = {
            convertLParamToPoint(lParam),
            Framework::currentTimestamp(),
        },
    };

    return touch;
}

LRESULT ManglMainWindow::renderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto framework = Framework::instance();

    const auto& layout = Env::layoutSize;

    try {
        switch (message) {
        case WM_SIZE: {
            if (wParam == SIZE_MINIMIZED)
                break;
            Rect clientRect{0, 0, Cord(LOWORD(lParam)), Cord(HIWORD(lParam))};
            processRenderLayoutChange(clientRect);
            mainLoopPendingLayout_ = true;
            return 0;
        }

        //case WM_RBUTTONDOWN:
        //    mouseRightDown_ = true;
        //    framework->onTouchDown(getMouseTouchInfo(lParam));
        //    //{
        //    //    auto touch = getMouseTouchesInfo(lParam);

        //    //    framework->onTouchDown(touch.touches[0]);



        //    //    touch.count = 2;
        //    //    auto& t = touch.touches[0];
        //    //    touch.touches[1] = touch.touches[0];
        //    //    touch.touches[1].p = {
        //    //        layout.w / 2 - (t.p.x - layout.w / 2) + layout.w / 2,
        //    //        layout.h / 2 - (t.p.y - layout.h / 2) + layout.h / 2};

        //    //    framework->onTouchesBegin(touch);
        //    //}
        //    return 0;

        //case WM_NCRBUTTONUP:
        //case WM_RBUTTONUP:
        //    mouseRightDown_ = false;
        //    framework->onTouchUp(getMouseTouchInfo(lParam));

        //    //{

        //    //auto touch = getMouseTouchesInfo(lParam);

        //    //framework->onTouchUp(touch.touches[0]);


        //    //touch.count = 2;
        //    //auto& t = touch.touches[0];
        //    //touch.touches[1] = touch.touches[0];
        //    //touch.touches[1].p = {
        //    //    layout.w / 2 - (t.p.x - layout.w / 2) + layout.w / 2,
        //    //    layout.h / 2 - (t.p.y - layout.h / 2) + layout.h / 2};

        //    //framework->onTouchesEnd(touch);
        //    return 0;
        ////}

        case WM_LBUTTONDOWN: {
            mouseLeftDown_ = true;

            auto touch = getMouseTouchInfo(lParam);

            framework->onTouchDown(touch);


            //auto touch = getMouseTouchesInfo(lParam);

            //framework->onTouchDown(touch.touches[0]);


            //framework->onTouchesBegin(touch);

            lastTouches_ = touch;


            //
            // Capture the mouse
            //
            RECT client;
            POINT point;

            GetClientRect(hwnd_, &client);

            point.x = client.left, point.y = client.top;
            ClientToScreen(hwnd_, &point);
            client.left = point.x, client.top = point.y;

            point.x = client.right, point.y = client.bottom;
            ClientToScreen(hwnd_, &point);
            client.right = point.x, client.bottom = point.y;

            ClipCursor(&client);
            return 0;
        }

        case WM_NCLBUTTONUP:
        case WM_LBUTTONUP:
            mouseLeftDown_ = false;

            // release the mouse
            ClipCursor(0);


            framework->onTouchUp(getMouseTouchInfo(lParam));

            //auto touch = getMouseTouchesInfo(lParam);
            //framework->onTouchUp(touch.touches[0]);


            //framework->onTouchesEnd(touch);
            return 0;

        case WM_MOUSELEAVE:
        case WM_NCMOUSEMOVE: {
            auto touch = getMouseTouchInfo(lParam);

            if (mouseLeftDown_) {
                //auto i = touch.count++;
                //touch.touches[i] = lastTouches_.touches[i];
                //touch.touches[i].timestamp = timestamp_;
                framework->onTouchUp(touch);
                mouseLeftDown_ = false;
            }

            //if (mouseRightDown_) {
            //    auto i = touch.count++;
            //    touch.touches[i] = lastTouches_.touches[i];
            //    touch.touches[i].timestamp = timestamp_;
            //    mouseRightDown_ = false;
            //}

            //if (touch.count > 0) {
            //    framework->onTouchUp(touch.touches[0]);

            //    framework->onTouchesEnd(touch);
            //}

            mouseTracking_ = false;
            return 0;
        }

        case WM_MOUSEMOVE:
            // Mouse tracking
            if (!mouseTracking_) {
                TRACKMOUSEEVENT tme{};
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hwnd_;
                if (TrackMouseEvent(&tme))
                    mouseTracking_ = true;
            }

            if ((wParam & MK_LBUTTON) == 0)
                mouseLeftDown_ = false;

            if ((wParam & MK_RBUTTON) == 0)
                mouseRightDown_ = false;

            if (mouseLeftDown_)
                framework->onTouchMove(getMouseTouchInfo(lParam));
            return 0;

        case WM_MOUSEWHEEL: {
            auto p = convertLParamToPoint(lParam);
            auto keys = GET_KEYSTATE_WPARAM(wParam);
            auto delta = (Cord)GET_WHEEL_DELTA_WPARAM(wParam);
            delta /= (Cord)WHEEL_DELTA;

            framework->onMouseWheel(delta, keys, p);
            return 0;
        }

        default: 
            break;
        }

    } catch (const std::exception& e) {
        handleWndProcException(e.what());
    } catch (...) {
        handleWndProcException("WndProc: Unhandled exception!");
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}


LRESULT ManglMainWindow::mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto framework = Framework::instance();

    //using LockGuard = MFrameworkNative::TimerLockGuard;
    //auto& mutex = MFrameworkNative::getTimerMutex();

    const auto& layout = Env::layoutSize;

    try {

        switch (message) {
            //
            // Creation, initialization
            //
        case WM_CREATE: {
            //hwnd_ = hWnd;
            // Creating menu
            // auto hw = CreateWindowExW ( 0, L"Static", L"aa", WS_CHILD |
            // WS_VISIBLE, 10, 50, 200, 20, hwnd_, HMENU(1), hInstance_, 0);

            break;
        } 

        case WM_SHOWWINDOW: {
            // Finishing layout
            //if (stateWindowFullScreen.get(false)) {
            //    setFullScreen(true);
            //}
            break;
        }

            //
            // Destruction
            //
        case WM_DESTROY: {
            // Save the window state
            stateWindowMaximized = isMaximized_;
            stateWindowFullScreen = isFullScreen_;

            if (!isMaximized_ && !isFullScreen_) {
                RECT wrect{};
                GetWindowRect(hwnd_, &wrect);
                stateWindowTopLeft = PointI{wrect.left, wrect.top};
                stateWindowSize =
                    SizeI{std::abs(wrect.right - wrect.left), std::abs(wrect.bottom - wrect.top)};
            }


            framework->onTerminate();
            destroyCleanup();

            if (htheme_)
                CloseThemeData(htheme_);

            if (exitOnClose)
                PostQuitMessage(0);
            return 0;
        }


            //
            // Layout
            //
        case WM_DPICHANGED:
            dpiChangePending_ = true;
            dpiChangeX_ = LOWORD(wParam);
            dpiChangeY_ = HIWORD(wParam);
            dpiSuggestedWinRect_ = *reinterpret_cast<RECT*>(lParam);
            processDpiChange();

            mainLoopPendingLayout_ = true;

            return 0;

        case WM_ENTERSIZEMOVE: break;

        case WM_EXITSIZEMOVE: {
            // if (dpiChangePending_) {
            //     dpiChangePending_ = false;
            //     mainLoopPendingLayout_ = true;
            //     //processDpiChange();
            // }

            // mainLoopPendingLayout_ = true;
            int a = 0;
            break;
        }

        case WM_SIZE: {
            if (wParam == SIZE_MINIMIZED)
                break;

            if (wParam == SIZE_MAXIMIZED)
                isMaximized_ = true;
            else
                isMaximized_ = false;

            Rect clientRect{0, 0, Cord(LOWORD(lParam)), Cord(HIWORD(lParam))};
            auto renderRect = computeRenderWindowLayout(clientRect);
            auto x = (int)renderRect.x;
            auto y = (int)renderRect.y;
            auto w = (int)renderRect.w;
            auto h = (int)renderRect.h;
            MoveWindow(renderHwnd_, x, y, w, h, false);
            InvalidateRect(hwnd_, NULL, TRUE);
            return 0;
        }


        // Keyboard support
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
            auto it = winScanCodes_.find(wParam);
            if (it == winScanCodes_.end())
                break;
            auto code = it->second;

            removePressedKey(code);
            currentPressedKeys_.push_back(code);

            //LockGuard lock{mutex};
            framework->onKeyDown(code);
            return 0;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP: {
            auto it = winScanCodes_.find(wParam);
            if (it == winScanCodes_.end())
                break;

            auto code = it->second;

            if (currentPressedKeys_ == framework->fullScreenShortcut_)
                toggleFullScreen();

            if (currentPressedKeys_ == framework->screenshotShortcut_)
                Framework::getRenderer()->takeScreenshot();

            if (currentPressedKeys_ == framework->backShortcut_)
                Framework::instance()->processBack();

            if (currentPressedKeys_ == framework->quitShortcut_) {
                DestroyWindow(hwnd_);
            }

            removePressedKey(code);

            //LockGuard lock{mutex};
            framework->onKeyUp(code);
            return 0;
        }


#if MANGL_MUSIC_MCI
        case MM_MCINOTIFY:
        case MCI_NOTIFY_ABORTED:
            if (mciDevice_)
                mciDevice_->onWndMessage(hWnd, message, wParam, lParam);
            break;
#endif
        case WM_COMMAND:
            for (auto& wit: winGuiWidgets_) {
                auto hwndCtl = (HWND)lParam;
                if (auto widget = wit.second; widget->hwnd == hwndCtl) {
                    switch (HIWORD(wParam)) {
                    case EN_CHANGE: {
                        WCHAR text[512];
                        if (GetWindowTextW(hwndCtl, text, 255) > 0) {
                            widget->text = apl::apiFromWin(text);

                            // TODO: implement callback
                            // LockGuard lock(mutex) ;
                            // framework->onInputTextChanged(, widget.param.text);
                        }
                    } break;
                    default: break;
                    }

                    break;
                }
            } // for all widgets
            break;

        case WM_CTLCOLORSTATIC: {
            //LockGuard lock{mutex};

            auto hdcStatic = (HDC)wParam;
            auto hwndStatic = (HWND)lParam;

            for (auto& wit: winGuiWidgets_) {
                if (auto widget = wit.second; widget->hwnd == hwndStatic) {
                    SetTextColor(hdcStatic, widget->textColor.getRGB());
                    SetBkColor(hdcStatic, widget->bkCol.getRGB());
                    if (widget->hbrush == 0)
                        widget->hbrush = CreateSolidBrush(widget->bkCol.getRGB());
                    return (INT_PTR)widget->hbrush;
                }
            }
            break;
        }

        case MANGL_WM_PROCESS_FRAMEWORK_ACTIONS: {
            auto dc = GetDC(hWnd);
            processFrameworkActions(dc);
            ReleaseDC(hWnd, dc);
            return 0;
        }

        default: break;
        }

    } catch (const std::exception& e) {
        handleWndProcException(e.what());
    } catch (...) {
        handleWndProcException("WndProc: Unhandled exception!");
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

void ManglMainWindow::handleWndProcException(StrView message) {
    ClipCursor(0); // Release the Mouse
    auto flags = MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND;
    apl::ApiString text{message};
    MessageBoxW(hwnd_, text.win(), L"Error!", flags);
    PostQuitMessage(0);
}


} // namespace mangl

#endif

