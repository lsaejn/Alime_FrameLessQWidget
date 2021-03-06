#include "Alime_WindowBase.h"
#include "Alime_TitleBar.h"
#include "Alime_ContentWidget.h"
#include "QFrameLessWidget_Alime.h"
#include "Alime_TransparentWidget.h"

#include <qpushbutton.h>

#include "qlayout.h"
#include "qicon.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

Alime_WindowBase::Alime_WindowBase(QWidget* parent, QLayout* ownerBox)
    : QWidget(parent),
    titleBar_(nullptr),
    box_(ownerBox)
{
    setAttribute(Qt::WA_StyledBackground, true);
    auto content = Alime_ContentWidget::creator_(this);

    titleBar_ = new Alime_TitleBar(this);
    installEventFilter(titleBar_);
    titleBar_->SysButtonEventRegister([=]() { box_->setMargin(content->GetShadowWidth());}, false);
    titleBar_->SysButtonEventRegister([=]() {box_->setMargin(0);}, true);

    setWindowTitle(content->GetTitle());
    setWindowIcon(QIcon(content->GetIcon()));
    parentWidget()->resize(content->GetWindowSize());

    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addWidget(titleBar_);
    pLayout->addWidget(content);

    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    Alime_TransparentWidget* ptr = (Alime_TransparentWidget*)parent;
    connect(ptr, &Alime_TransparentWidget::windowMoved, titleBar_, &Alime_TitleBar::appWindowMoved);
}


