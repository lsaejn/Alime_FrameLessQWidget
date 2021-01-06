#include "QFrameLessWidget_Alime.h"
#include <QLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QProgressBar>

CLASSREGISTER(QFrameLessWidget_Alime)

QFrameLessWidget_Alime::QFrameLessWidget_Alime(QWidget *parent)
    : Alime_ContentWidget(parent)
{
    //return;
    QHBoxLayout* contentLayout = new QHBoxLayout(this);
    QWidget* leftContent = new QWidget(this);
    QWidget* rightContent = new QWidget(this);

    contentLayout->addWidget(leftContent);
    contentLayout->addWidget(rightContent);
    contentLayout->setMargin(0);
    contentLayout->setSpacing(0);
    contentLayout->setStretch(0, 1);
    contentLayout->setStretch(1, 4);

    
    leftContent->setObjectName("leftContent");
    //leftContent->setFixedWidth(240);

    rightContent->setObjectName("rightContent");

    
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContent);
    leftLayout->setMargin(0);

    
    QPushButton* btn01 = new QPushButton("click me");
    btn01->setObjectName("btnBoard");
    btn01->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftLayout->addWidget(btn01);

    QPushButton* btn02 = new QPushButton("click me");
    btn02->setObjectName("btnBoard");
    leftLayout->addWidget(btn02);
    btn02->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton* btn03= new QPushButton("click me");
    btn03->setObjectName("btnBoard");
    btn03->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftLayout->addWidget(btn03);

    QPushButton* btn04 = new QPushButton("click me");
    btn04->setObjectName("btnBoard");
    btn04->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftLayout->addWidget(btn04);


    QVBoxLayout* vbox = new QVBoxLayout(rightContent);
    //vbox->setMargin(20);//setMargin只有box才能用，指定box到父widget的边距
    vbox->setContentsMargins(10, 0, 0, 0);//
    vbox->setSpacing(0);
    setContentsMargins(10, 10, 10, 10);//这一句是widget内部所有内容的边距

    vbox->addWidget(new QPushButton("fuck1"));
    vbox->addWidget(new QPushButton("fuck2"));
    vbox->addStretch(1);
}

#include <QPainter>
void QFrameLessWidget_Alime::paintEvent(QPaintEvent* event)
{
    //Alime_ContentWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawLine(80, 100, 650, 500);
    painter.setPen(Qt::red);
    painter.drawRect(10, 10, 100, 400);
    painter.setPen(QPen(Qt::green, 5));
    painter.setBrush(Qt::blue);
    painter.drawEllipse(50, 150, 400, 200);



    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255,0,0));
    painter.setPen(Qt::DotLine);
    painter.setBrush(brush);
    painter.drawEllipse(0,0,
        200, 200);

  
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    auto w = width();
    path.addEllipse(w / 2, height() / 2, width(), height());
    painter.setClipPath(path);

    painter.drawPixmap(QRect(width() / 2, height() / 2, width(), height()),
        QPixmap(":/images/min-128.png"));
    
}
