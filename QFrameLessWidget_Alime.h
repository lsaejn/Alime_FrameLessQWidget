#pragma once

#include <QtWidgets/QWidget>
#include "Alime_ContentWidget.h"


class QFrameLessWidget_Alime : public Alime_ContentWidget
{
    Q_OBJECT

public:
    QFrameLessWidget_Alime(QWidget *parent = Q_NULLPTR);
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
private:

};
