#ifndef LEDINDICATORWIDGET_H
#define LEDINDICATORWIDGET_H

#include <QWidget>
#include <QPainter>

class LedIndicatorWidget : public QWidget {
    Q_OBJECT
public:
    explicit LedIndicatorWidget(QWidget *parent = 0);
    void setOn(bool enabled);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    bool state;
};

#endif // LEDINDICATORWIDGET_H
