#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include "Alime_TitleBar.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif


class Alime_TitlePushButton
{
public:
    static void InitPushButton(QPushButton* btn, const QString& fileUrl,
        const QString& tooltip, const QString& objName,
        const size_t btnSize, const size_t imageSize, bool show = true)
    {
        btn->setObjectName(objName);
        btn->setFixedSize(btnSize, btnSize);
        btn->setIconSize(QSize(imageSize, imageSize));
        show ? btn->show() : btn->hide();
        btn->setIcon(QIcon(fileUrl));
        btn->setToolTip(tooltip);
    }
};


Alime_TitleBar::Alime_TitleBar(QWidget* parent)
    : QWidget(parent),
    IsNcPressing_(false)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_StyledBackground, true);

    titleLabel_ = new QLabel(this);
    iconLabel_ = new QLabel(this);
    closeButton_ = new QPushButton(this);
    minimizeButton_ = new QPushButton(this);
    maximizeButton_ = new QPushButton(this);

    iconLabel_->setFixedSize(32, 32);
    iconLabel_->setObjectName("titleLabel");
    iconLabel_->setScaledContents(true);

    titleLabel_->setObjectName("titleLabel");
    titleLabel_->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    InitPushButton();

    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(iconLabel_);
    pLayout->addSpacing(10);
    pLayout->addWidget(titleLabel_);
    pLayout->addStretch();
    pLayout->addWidget(minimizeButton_);
    pLayout->addWidget(maximizeButton_);
    pLayout->addWidget(closeButton_);
    pLayout->setSpacing(10);
    pLayout->setContentsMargins(5, 0, 5, 0);
}

Alime_TitleBar::~Alime_TitleBar()
{

}


void Alime_TitleBar::mousePressEvent(QMouseEvent* event)
{
#ifdef Q_OS_WIN
    
    if (ReleaseCapture() && event->button()== Qt::LeftButton)
    {
        IsNcPressing_ = true;
    }
    event->ignore();
#else
#endif
}
#include <QDebug>
void Alime_TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (IsNcPressing_)
    {
        qDebug() << "mouseMoveEvent";
        PostMessage(HWND(this->window()->winId()), WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }
}


void Alime_TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
#ifdef Q_OS_WIN
    if (IsNcPressing_ )
    {
        event->ignore();
    }
    IsNcPressing_ = false;
    static ULONGLONG lastClick = -1;
    ULONGLONG thisClick = GetTickCount64();
    ULONGLONG interval = thisClick - lastClick;
    lastClick = thisClick;
    if (interval < 500)
    {
        emit maximizeButton_->clicked();
    }
#else
#endif
}


bool Alime_TitleBar::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget* pWidget = qobject_cast<QWidget*>(obj);
        if (pWidget)
        {
            QString titleName = pWidget->windowTitle();
            titleLabel_->setText(titleName);
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget* pWidget = qobject_cast<QWidget*>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            iconLabel_->setPixmap(icon.pixmap(iconLabel_->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void Alime_TitleBar::onClicked()
{
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    QWidget* pWindow = this->window();
    auto name = pButton->objectName();
    if (pWindow->isTopLevel())
    {
        if (pButton->objectName()=="minimizeButton")
        {
            pWindow->showMinimized();
        }
        else if (pButton->objectName() == "maximizeButton" || pButton->objectName() == "resetButton")
        {
            if (maximizeButton_->objectName()=="maximizeButton")
            {
                if(maxStateCallback_)
                    maxStateCallback_();
               
            }
            else if(normalStateCallback_)
            {
                normalStateCallback_();
            }
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton->objectName() == "closeButton")
        {
            pWindow->close();
        }
    }
}

void Alime_TitleBar::updateMaximize()
{
    if (IsNcPressing_)
    {
        normalStateCallback_();
        return;
    }
        
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            maximizeButton_->setIcon(QIcon(":/images/reset.png"));
            maximizeButton_->setObjectName("resetButton");
            qDebug() << "fuck2";
        }
        else
        {
            maximizeButton_->setIcon(QIcon(":/images/max.png"));
            maximizeButton_->setObjectName("maximizeButton");
        }
    }
}

void Alime_TitleBar::InitPushButton()
{
    Alime_TitlePushButton::InitPushButton(minimizeButton_, ":/images/min.png", "Minimize", "minimizeButton", 40, 32);
    Alime_TitlePushButton::InitPushButton(maximizeButton_, ":/images/max.png", "Maximize", "maximizeButton", 40, 32);
    Alime_TitlePushButton::InitPushButton(closeButton_, ":/images/close.png", "Close", "closeButton", 40, 32);

    connect(minimizeButton_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(maximizeButton_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(closeButton_, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

bool Alime_TitleBar::SysButtonEventRegister(WindowStateChangeCallback f, bool IsMaxstate)
{
    if (IsMaxstate)
        maxStateCallback_ = f;
    else
        normalStateCallback_ = f;
    return true;
}

void Alime_TitleBar::appWindowMoved()
{
    maximizeButton_->setIcon(QIcon(":/images/max.png"));
    maximizeButton_->setObjectName("maximizeButton");
}

//#include <QStyleOption>
//#include <QPainter>
//void Alime_TitleBar::paintEvent(QPaintEvent* event)
//{
    //Q_UNUSED(event);
    //QStyleOption styleOpt;
    //styleOpt.init(this);
    //QPainter painter(this);
    //style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);
//}
