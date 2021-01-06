#include "Alime_TransparentWidget.h"
#include "Alime_WindowBase.h"
#include <QLayout>
#include <QGraphicsDropShadowEffect>
#include <windows.h>        //注意头文件
#include <windowsx.h>
#include <QMouseEvent>

Alime_TransparentWidget::Alime_TransparentWidget(QWidget* parent)
	:QWidget(parent),
    boundaryWidth(5)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //阴影
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setObjectName("border");
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#FF444444"));
    //阴影圆角
    shadow->setBlurRadius(10);
    QVBoxLayout* box = new QVBoxLayout(this);
    base= new Alime_WindowBase(this, box);
    
    box->addWidget(base);
    //给嵌套QWidget设置阴影
    //base->setGraphicsEffect(shadow);
    box->setMargin(5);
}


bool Alime_TransparentWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = (MSG*)message;
    auto w=width();
    auto h = height();
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
        if (xPos < boundaryWidth && yPos < boundaryWidth)                    //左上角
            *result = HTTOPLEFT;
        else if (xPos >= width() - boundaryWidth && yPos < boundaryWidth)          //右上角
            *result = HTTOPRIGHT;
        else if (xPos < boundaryWidth && yPos >= height() - boundaryWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if (xPos >= width() - boundaryWidth && yPos >= height() - boundaryWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if (xPos < boundaryWidth)                                     //左边
            *result = HTLEFT;
        else if (xPos >= width() - boundaryWidth)                              //右边
            *result = HTRIGHT;
        else if (yPos < boundaryWidth)                                       //上边
            *result = HTTOP;
        else if (yPos >= height() - boundaryWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;
        return true;
    }
    return false;         //此处返回false，留给其他事件处理器处理
}

#include <QPainter>
#include <QtMath>
void Alime_TransparentWidget::paintEvent(QPaintEvent* event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QRectF rect(10, 10, this->width() - 20, this->height() - 20);
    path.addRoundRect(rect, 8, 8);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for (int i = 0; i < 10; i++) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
        color.setAlpha(150 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}