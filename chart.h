#ifndef CHART_H
#define CHART_H

#include <QApplication>
#include <QGraphicsView>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>

class Chart : public QGraphicsView {
public:
    Chart(QGraphicsScene *scene, QObject *parent = 0);
    QPointF getScenePos();
    bool checkScale(qint32 delta);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QObject *parent;
    qint32 mouseOriginX;
    qint32 mouseOriginY;
    QPointF scenePos;

};

#endif // CHART_H
