#include "QFrameLessWidget_Alime.h"
#include "Alime_TransparentWidget.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qssfile(":/qss/dark.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    a.setStyleSheet(qss);
    QFont font("Microsoft YaHei");
    font.setPointSize(8);
    a.setFont(font);

    Alime_TransparentWidget w;
    w.show();
    return a.exec();
}
