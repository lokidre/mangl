/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "animation_widget.h"

#include <mangl/main_window.h>

APEAL_QT_HEADERS_BEGIN
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QFileDialog>
APEAL_QT_HEADERS_END

#include "animation_screen.h"
#include "main_window.h"
#include "app.h"
#include "animator_mode.h"
#include "storage.h"

namespace animator {

AnimatorScreenWidget::AnimatorScreenWidget(QWidget* parent): Base{parent}
{
    manglWindow_ = ManglMainWindow::instance();

    createScreen();

    //setLineWidth(frameLineWidth);
    //setFrameStyle(QFrame::Box);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void AnimatorScreenWidget::createScreen()
{
    screenSize = Env::applicationRect.size();
    //screenSize = Env::layoutSize; //* env->stretchValue ;
    //Env::setApplicationSize(screenSize);
    //env->applicationFrame.resize(env->layoutSize);

    manglWindow_->exitOnClose = false;

    screenSize *= Env::pixelDensity;

    auto windowRect = RectI::make(0, 0, screenSize.w, screenSize.h);

    //mangl::mangl_main();
    //mangl::MFrameworkNative::instance()->init();

    manglWindow_->createMainWindow((HWND)this->winId(), windowRect, WS_CHILD | WS_VISIBLE);
    //SetParent(manglWindow_->getSystemHandle(), (HWND)this->winId());

    //manglWindow_->createQt(this, windowRect, 0);

}


void AnimatorScreenWidget::onResolutionChanged()
{
    //manglWindow_->destroyMainWindow();
    //App::instance()->reset();
    //createScreen();
    screenSize = Env::applicationRect.size();
    screenSize *= Env::pixelDensity;

    auto windowRect = RectI::make(0, 0, screenSize.w, screenSize.h);

    MoveWindow(manglWindow_->getSystemHandle(), 0, 0, windowRect.w, windowRect.h, FALSE);

    updateGeometry();
}




AnimatorWidget::AnimatorWidget(QWidget* parent): QFrame{parent}
{
    using namespace apl;

    auto settings = Settings::instance();

    mainLayout_ = new QVBoxLayout;

    auto screenWidgetLayout = new QHBoxLayout;

    screenWidget_ = new AnimatorScreenWidget;

    screenWidgetLayout->setContentsMargins(12, 12, 12, 12);
    screenWidgetLayout->addStretch();
    screenWidgetLayout->addWidget(screenWidget_);
    screenWidgetLayout->addStretch();

    mainLayout_->addLayout(screenWidgetLayout);


    screen_ = app->animatorScreen;


    auto timer = new QTimer{this};
    connect(timer, &QTimer::timeout, this, &Self::onTimer);
    timer->start(1000);



    //layout->addSpacing(env->layout_height+40) ;
    mainLayout_->addStretch();


    auto controlsLayout = new QHBoxLayout();

    // Debug group
    {
        auto group = new QGroupBox("Debug");
        auto layout = new QGridLayout;
        int col{}, row{};

        //
        // First column
        //
        auto checkbox = new QCheckBox("Wireframe");
        connect(checkbox, &QCheckBox::clicked, [this](bool st) {
            App::LockGuard lock{App::getMainMutex()};
            app->animatorScreen->onWireframeEnable(st);
        });
        layout->addWidget(checkbox, row++, col);
        wireframeCheckbox_ = checkbox;

        checkbox = new QCheckBox("Vertices");
        verticesCheckbox_ = checkbox;
        connect(checkbox, &QCheckBox::clicked, [this](bool st) { 
            App::LockGuard lock{App::getMainMutex()};
            app->animatorScreen->onVerticesEnable(st); 
        });
        layout->addWidget(checkbox, row++, col);

        checkbox = new QCheckBox("Axes");
        axesCheckbox_ = checkbox;
        layout->addWidget(checkbox, row++, col);


        //
        // Second column
        //
        col = 1;
        row = 0;
        checkbox = new QCheckBox("Bounds");
        boundsCheck_ = checkbox;
        connect(checkbox, &QCheckBox::clicked, [this](bool st) {
            App::LockGuard lock{App::getMainMutex()};
            app->animatorScreen->onBoundsEnable(st); 
        });
        layout->addWidget(checkbox, row++, col);

        checkbox = new QCheckBox("Labels");
        labelsCheck_ = checkbox;
        connect(checkbox, &QCheckBox::clicked, [this](bool st) {
            App::LockGuard lock{App::getMainMutex()};
            app->animatorScreen->onLabelsEnable(st); 
        });
        layout->addWidget(checkbox, row++, col);


        group->setLayout(layout);
        controlsLayout->addWidget(group);
    }


    // Lighting group
    {
        auto group = new QGroupBox("Lighting");
        lightingGroup_ = group;
        group->setCheckable(true);

        auto layout = new QGridLayout;
        int row{}, col{};

        screen_->onLightingEnable(true);
        group->setChecked(true);

        connect(group, &QGroupBox::toggled, [this](bool st) { 
            App::LockGuard lock{App::getMainMutex()};
            screen_->onLightingEnable(st);  
        });

        auto checkbox = new QCheckBox("Manual Position:");

        checkbox->setChecked(false);
        screen_->onUserLightPositionEnable(false);

        connect(checkbox, &QCheckBox::clicked, [this](bool st) {	
            App::LockGuard lock{App::getMainMutex()};
            screen_->onUserLightPositionEnable(st);  
        });

        layout->addWidget(checkbox, row++, col);


        int positions[] = {-500, 500, 500};
        screen_->setUserLightPositionI({positions[0],positions[1],positions[2]});

        for (int i = 0; i < 3; ++i) {
            auto spinbox = new QSpinBox;
            lightPosition_[i] = spinbox;
            spinbox->setSingleStep(10);
            spinbox->setRange(-1000, 1000);
            spinbox->setValue(positions[i]);
            connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
                App::LockGuard lock{App::getMainMutex()};
                screen_->setUserLightPositionI({
                    lightPosition_[0]->value(), 
                    lightPosition_[1]->value(), 
                    lightPosition_[2]->value()});
            });
            layout->addWidget(lightPosition_[i], row, i+1);
        }
        ++row;


        group->setLayout(layout);
        controlsLayout->addWidget(group);
    }

    // Layout group
    {
        auto group = new QGroupBox("Custom Layout");
        auto layout = new QGridLayout;

        group->setCheckable(true);
        group->setChecked(settings->customLayoutOn());

        connect(group, &QGroupBox::toggled, [this, settings](bool on) {
            settings->customLayoutOn = on;
            AnimatorMode::instance()->onUpdate();
        });

        auto label = new QLabel("Size:");
        layout->addWidget(label, 0, 0);

        auto customSize = settings->customLayoutSize();

        customLayoutWidth_ = new QLineEdit;
        customLayoutWidth_->setValidator(new QIntValidator(10, 2000));
        customLayoutWidth_->setText(QString("%1").arg(customSize.w));
        layout->addWidget(customLayoutWidth_, 0, 1);


        //label = new QLabel("Height:") ;
        //groupLayout->addWidget(label,0,1) ;

        customLayoutHeight_ = new QLineEdit;
        customLayoutHeight_->setValidator(new QIntValidator(10, 2000));
        customLayoutHeight_->setText(QString("%1").arg(customSize.h));
        layout->addWidget(customLayoutHeight_, 0, 2);


        auto updateButton = new QPushButton("Update");
        connect(updateButton, &QPushButton::clicked, this, [this, settings]() {
            settings->customLayoutSize = SizeI{
                customLayoutWidth_->text().toInt(),
                customLayoutHeight_->text().toInt()};
            AnimatorMode::instance()->onUpdate();
            onResolutionChanged();
        });
        layout->addWidget(updateButton, 1, 0, 1, 2);

        group->setLayout(layout);
        controlsLayout->addWidget(group);
    }


    controlsLayout->addStretch();
    mainLayout_->addLayout(controlsLayout);

    // Textures section
    {
        auto layout = new QHBoxLayout;
        auto group = new QGroupBox("Textures");
        group->setLayout(layout);

        auto label = new QLabel("Texture Dir:");
        layout->addWidget(label);

        textureDirLabel_ = new QLabel;
        layout->addWidget(textureDirLabel_, 10);

        textureDirButton_ = new QPushButton("Pick...");
        textureDirButton_->setEnabled(false);

        connect(textureDirButton_, &QPushButton::clicked, [this] {
            auto animator = Animator::instance();
            auto& item = animator->getCurrentItem();
            auto title = "Select Texture Directory";

            auto dir = QFileDialog::getExistingDirectory(this, title, toQt(item.textureDir));
            if (!dir.isEmpty()) {

                item.textureDir = fromQt(dir);
                textureDirLabel_->setText(dir);

                animator->saveState();
            }
            
        });
        layout->addWidget(textureDirButton_);



        mainLayout_->addWidget(group);

    }


    // progress slider
    {
        progressSlider_ = new QSlider(Qt::Horizontal);
        progressSlider_->setTickPosition(QSlider::TicksBelow);
        mainLayout_->addWidget(progressSlider_);
    }


    // Control buttons
    {
        auto layout = new QHBoxLayout;

        playButton_ = new QPushButton("Play", this);
        layout->addWidget(playButton_);

        stopButton_ = new QPushButton("Stop", this);
        layout->addWidget(stopButton_);

        layout->addStretch();

        clearButton_ = new QPushButton("Clear", this);
        connect(clearButton_, &QPushButton::clicked, [this] {
            app->animatorScreen->clearAll();
        });
        layout->addWidget(clearButton_);

        layout->addStretch();

        repeatCheckbox_ = new QCheckBox("Repeat", this);
        layout->addWidget(repeatCheckbox_);


        mainLayout_->addLayout(layout);
    }



    setLayout(mainLayout_);
}


void AnimatorWidget::onUpdateContents()
{
    auto animator = Animator::instance();

    if (animator->isItemSelected()) {
        textureDirButton_->setEnabled(true);
        auto& item = animator->getCurrentItem();
        textureDirLabel_->setText(toQt(item.textureDir));
    } else {
        textureDirButton_->setEnabled(false);
        textureDirLabel_->clear();
    }



    //if (animator->currentTab()

    //if (app->
    //mainWindow
    
    /*
    if ( mainWindow_->selectWidget->currentTab() == TAB_MODEL ) {
        auto lightPosition = manglFramework_->scene()->lightPosition() ;
        lightPosition_[0]->setValue((int)lightPosition.x) ;
        lightPosition_[1]->setValue((int)lightPosition.y) ;
        lightPosition_[2]->setValue((int)lightPosition.z) ;
    }
    */
}


void AnimatorWidget::onTimer()
{
    if (auto screen = app->animatorScreen; screen && screen->errorFlag) {
        screen->errorFlag = false;
        QMessageBox::critical(this, "Error!", toQt(screen->errorMessage), QMessageBox::Ok);
    }
}

void AnimatorWidget::onResolutionChanged()
{
    screenWidget_->onResolutionChanged();
    mainLayout_->invalidate();
    mainLayout_->activate();
}

//void AnimatorWidget::onWireframeCheckbox()
//{
//	manglFramework_->animatorScreen->onWireframe(wireframeCheckbox_->isChecked()) ;
//}

} // namespace animator
