#include "Alime_TransparentWidget.h"
#include "Alime_WindowBase.h"
#include <QLayout>
#include <QGraphicsDropShadowEffect>
#include <windows.h>        //注意头文件
#include <windowsx.h>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>

Alime_TransparentWidget::Alime_TransparentWidget(QWidget* parent)
	:QWidget(parent),
    boundaryWidth_(8)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //阴影
    //QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    //shadow->setObjectName("border");
    //shadow->setOffset(0, 0);
    //shadow->setColor(QColor("#FF444444"));
    //shadow->setBlurRadius(10);


    QVBoxLayout* box = new QVBoxLayout(this);
    base_= new Alime_WindowBase(this, box);
    //base_->setGraphicsEffect(shadow);
    
    box->addWidget(base_);
    box->setMargin(5);
}


bool Alime_TransparentWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = (MSG*)message;
    switch (msg->message)
    {
    case WM_MOVE:
    {
        emit windowMoved();
    }
    break;
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if (xPos < boundaryWidth_ && yPos < boundaryWidth_) 
            *result = HTTOPLEFT;
        else if (xPos >= width() - boundaryWidth_ && yPos < boundaryWidth_)
            *result = HTTOPRIGHT;
        else if (xPos < boundaryWidth_ && yPos >= height() - boundaryWidth_)
            *result = HTBOTTOMLEFT;
        else if (xPos >= width() - boundaryWidth_ && yPos >= height() - boundaryWidth_)
            *result = HTBOTTOMRIGHT;
        else if (xPos < boundaryWidth_)
            *result = HTLEFT;
        else if (xPos >= width() - boundaryWidth_)
            *result = HTRIGHT;
        else if (yPos < boundaryWidth_)
            *result = HTTOP;
        else if (yPos >= height() - boundaryWidth_)
            *result = HTBOTTOM;
        else
            return false;
        return true;
    }
    return false;
}

void Alime_TransparentWidget::paintEvent(QPaintEvent* event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF rect(10, 10, qreal(this->width()) - 20, qreal(this->height()) - 20);
    path.addRoundRect(rect, 8, 8);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for (int i = 0; i < 10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
        color.setAlpha(150 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}