/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main.h"

#pragma warning(push, 0)
#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>

#if TARGET_OS_WINDOWS && !defined(_DLL)
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

#pragma warning(pop)

#include "main_window.h"

namespace animator {

class Application: public QApplication {
public:
    Application(int argc, char** argv): QApplication(argc, argv) { }
    virtual ~Application() {}

    /*
    virtual bool notify(QObject* rec, QEvent* ev)
    {
        try {
            return QApplication::notify(rec, ev);
        }
        catch (std::exception& e) {
            QMessageBox::critical(0, "Error!", e.what());
        }
        catch (...) {
            QMessageBox::critical(0, "Error!", "Unhandled Exception!");
        }
        return false;
    }
     */

    void on_quit() {}
};


static auto appStyleSheet = R"(
QSplitter::handle:horizontal {
	background: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 #eee, stop:1 #ccc);
	border: 1px solid #777;
	width: 13px;
	margin-top: 20px;
	margin-bottom: 20px;
	border-radius: 4px;
}
)";

}  // namespace animator


int main(int argc, char** argv)
{
    using namespace animator;

    int retCode = 1;

    try {
        Application a(argc, argv);

        a.setStyleSheet(appStyleSheet);

        QCoreApplication::setOrganizationName(PACKAGE_COMPANY);
        QCoreApplication::setOrganizationDomain(PACKAGE_COMPANY_DOMAIN);
        QCoreApplication::setApplicationName(PACKAGE_PRODUCT);

        mangl_main();
        mangl_framework_->init();


        auto w = new MainWindow;
        w->show();

        retCode = a.exec();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(0, "Error!", e.what());
    }
    catch (...) {
        QMessageBox::critical(0, "Error!", "Unhandled Exception!");
    }

    return retCode;

}

