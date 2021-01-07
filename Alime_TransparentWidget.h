#pragma once
#include <QWidget>

class Alime_WindowBase;
class Alime_TransparentWidget : public QWidget
{
    Q_OBJECT

signals:
    void windowMoved();

public:
    Alime_TransparentWidget(QWidget* parent = Q_NULLPTR);
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
private:
    size_t boundaryWidth_;
    Alime_WindowBase* base_;
};
