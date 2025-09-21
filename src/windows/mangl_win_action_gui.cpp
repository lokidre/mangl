/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/native/mangl_native_gui.h"

#include "mangl_win_main_window.h"
#include "../core/mangl_framework.h"

#if MANGL_WINMAIN

namespace mangl {

using ApiString = apl::ApiString;

void NativeGui::alert(AlertParamCR parms) {
    MessageBoxW(
        ManglMainWindow::instance()->hwnd(), 
        ApiString{parms.message}.win(), ApiString{parms.title}.win(),
        MB_OK | MB_ICONERROR
    );

    //// TODO: redo with objects
    //MFrameworkNative::instance()->pushFrameworkAction(
    //    MActionCode::GUI_ALERT, parms.message, 0, parms.title
    //);
}

WidgetId NativeGui::inputDialog(InputDialogParamA param) {
    Framework::instance()->pushFrameworkAction(
        MActionCode::GUI_PROMPT_TEXT_INPUT, param.prompt, 0, param.title
    );
    return {};
}

auto NativeGui::clearWidgets() -> SelfRet {
    for (auto& widget: ManglMainWindow::instance()->winGuiWidgets_)
        widget.second->remove = true;
    Framework::instance()->pushFrameworkAction(MActionCode::GUI_CLEAR);
    return this;
}

auto NativeGui::setVisible(WidgetId widgetId, bool visible) -> SelfRet {
    Framework::instance()->pushFrameworkAction(
        MActionCode::GUI_SET_VISIBLE, size_t(visible), 0, widgetId
    );
    return this;
}

auto NativeGui::move(WidgetId widgetId, RectA rect) -> SelfRet {
    Framework::instance()->pushFrameworkAction(MActionCode::GUI_MOVE, rect, widgetId);
    return this;
}

WidgetId NativeGui::createInput(StrView text, RectA contentsFrame, Flags flags) {
    auto widget =
        ManglMainWindow::instance()->newWidget(ManglMainWindow::WinGuiWidget::Type::Input);
    widget->text = text;
    widget->contentsFrame = contentsFrame;
    widget->flags = flags;

    Framework::instance()->pushFrameworkAction(MActionCode::GUI_CREATE_TEXT_INPUT, widget->id);

    return widget->id;
}

void NativeGui::setInputPlaceholder(WidgetId, StrView) {}

WidgetId NativeGui::createLabel(StrView text, RectA rect, Flags flags) {
    auto mainWindow = ManglMainWindow::instance();

    auto widget = mainWindow->newWidget(ManglMainWindow::WinGuiWidget::Type::Label);
    widget->text = text;
    widget->contentsFrame = rect;
    widget->flags = flags;

    Framework::instance()->pushFrameworkAction(MActionCode::GUI_CREATE_LABEL, widget->id);

    return widget->id;
}

auto NativeGui::setText(WidgetId widgetId, StrView text) -> SelfRet {
    auto widget = ManglMainWindow::instance()->widgetById(widgetId);

    widget->text = text;

    Framework::instance()->pushFrameworkAction(MActionCode::GUI_SET_TEXT, text, widget->id);
    return this;
}

auto NativeGui::setFont(WidgetId widgetId, Cord size, StrView fontName) -> SelfRet {
    Framework::instance()->pushFrameworkAction(
        MActionCode::GUI_SET_FONT, size_t(size), 0, widgetId, fontName
    );
    return this;
}

auto NativeGui::setTextColor(WidgetId widgetId, ColorA color) -> SelfRet {
    auto widget = ManglMainWindow::instance()->widgetById(widgetId);
    widget->textColor = color;
    widget->invalidate();
    return this;
}

auto NativeGui::setBackgroundColor(WidgetId widgetId, ColorA color) -> SelfRet {
    auto widget = ManglMainWindow::instance()->widgetById(widgetId);
    widget->bkCol = color;
    widget->invalidate();
    return this;
}

String NativeGui::getText(WidgetId widgetId) {
    // using namespace mangl;
    // LockGuard lock(callbackMutex_);
    auto widget = ManglMainWindow::instance()->widgetById(widgetId);
    MDebugTrapAssert(widget);

    return widget->text;
}


bool ManglMainWindow::processFrameworkActionsGui(HDC hdc, const MFrameworkAction& action) {
    using namespace apl;

    using C = MActionCode;

    switch (action.code) {
    case C::GUI_ALERT:
        MessageBoxW(
            hwnd_, ApiString{action.param}.win(), ApiString{action.param2}.win(),
            MB_OK | MB_ICONERROR
        );
        break;

    case C::GUI_CLEAR:
        for (auto wit = winGuiWidgets_.begin(); wit != winGuiWidgets_.end();) {
            if (auto widget = wit->second; widget->remove) {
                if (widget->hbrush)
                    DeleteObject(widget->hbrush);
                else if (widget->hfont)
                    DeleteObject(widget->hfont);
                DestroyWindow(widget->hwnd);
                wit = winGuiWidgets_.erase(wit);
            } else {
                ++wit;
            }
        }
        break;

    case C::GUI_SET_VISIBLE:
        if (auto widget = widgetById((CommonId)action.data); widget != 0) {
            bool visible = action.dataSize ? true : false;
            LONG dwStyle = GetWindowLongW(widget->hwnd, GWL_STYLE);
            if (visible) {
                dwStyle |= WS_VISIBLE;
                widget->flags |= M::Visible;
                widget->flags &= ~M::Hidden;
            } else {
                dwStyle &= ~WS_VISIBLE;
                widget->flags |= M::Hidden;
                widget->flags &= ~M::Visible;
            }
            SetWindowLongW(widget->hwnd, GWL_STYLE, dwStyle);
            widget->invalidate();
        }
        break;


    case C::GUI_CREATE_TEXT_INPUT:
        if (auto widget = widgetById((WidgetId)action.data); widget != 0) {
            auto wrect = _MRectToWindowsRect(widget->contentsFrame);

            widget->hwnd = CreateWindowExW(
                0, L"Edit", ApiString(widget->text).win(), WS_CHILD | WS_VISIBLE, wrect.x, wrect.y,
                wrect.w, wrect.h, hwnd_, (HMENU)(UINT_PTR)widget->id, hInstance_, 0
            );

            widget->invalidate();
        }
        break;

    case C::GUI_CREATE_LABEL:
        if (auto widget = widgetById((WidgetId)action.data); widget != 0) {
            DWORD exStyle = 0; // WS_EX_TRANSPARENT ;

            // auto ptext = toSysApiString(widget->text).c_str();
            auto wrect = getTextBoundary(hdc, widget->text, widget->contentsFrame);

            DWORD dwStyle = WS_CHILD;
            if ((widget->flags & M::Hidden) == 0)
                dwStyle |= WS_VISIBLE;

            widget->hwnd = CreateWindowExW(
                exStyle, L"Static", ApiString{widget->text}.win(), dwStyle, wrect.x, wrect.y,
                wrect.w, wrect.h, hwnd_, (HMENU)(UINT_PTR)widget->id, hInstance_, 0
            );

            updateWidgetRect(hdc, widget);

            widget->invalidate();
        }
        break;

    case C::GUI_SET_TEXT:
        if (auto widget = widgetById((WidgetId)action.data); widget != 0) {
            widget->text = action.param;
            SetWindowTextW(widget->hwnd, ApiString{widget->text}.win());
            updateWidgetRect(hdc, widget);
            widget->invalidate();
        }
        break;

    case C::GUI_MOVE:
        if (auto widget = widgetById((WidgetId)action.data); widget) {
            widget->contentsFrame = action.contentsFrame;
            updateWidgetRect(hdc, widget);
            widget->invalidate();
        }
        break;

    case C::GUI_SET_FONT:
        if (auto widget = widgetById((WidgetId)action.data); widget) {
            int fontHeight = int(double(action.dataSize) * 1.3);
            if (widget->hfont)
                DeleteObject(widget->hfont);

            auto& fontName = action.param;
            widget->hfont = CreateFontW(
                fontHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH,
                fontName.empty() ? L"Helvetica" : ApiString{fontName}.win()
            );
            SelectObject(hdc, widget->hfont);
            SendMessageW(widget->hwnd, WM_SETFONT, (WPARAM)widget->hfont, MAKELPARAM(TRUE, 0));
            updateWidgetRect(hdc, widget);
            widget->invalidate();
        }
        break;

    default: return false;
    }

    return true;
}

} // namespace mangl

#endif
