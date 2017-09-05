#include "chart.h"

/**
 * @brief Chart viewer constructor
 * @param scene
 * @param parent
 */
Chart::Chart(QGraphicsScene *scene, QObject *parent) : parent(parent){
    setCursor(Qt::CrossCursor);
    horizontalScrollBar()->setCursor(Qt::ArrowCursor);
    verticalScrollBar()->setCursor(Qt::ArrowCursor);
    setScene(scene);
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

/**
 * @brief Mouse press event
 * @param event
 */
void Chart::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::MiddleButton) {
        mouseOriginX = event->x();
        mouseOriginY = event->y();
    } else if(event->button() == Qt::RightButton) {
        if(parent != nullptr) {
            scenePos = mapToScene(event->pos());
            qApp->sendEvent(parent, event);
        }
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

/**
 * @brief Mouse move event
 * @param event
 */
void Chart::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::MidButton) {
        QPointF oldP = mapToScene(mouseOriginX, mouseOriginY);
        QPointF newP = mapToScene(event->pos());
        QPointF translation = newP - oldP;
        translate(translation.x(), translation.y());
        mouseOriginX = event->x();
        mouseOriginY = event->y();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

/**
 * @brief Wheel event (for zoom with Ctrl)
 * @param event
 */
void Chart::wheelEvent(QWheelEvent *event) {
    if(event->modifiers() & Qt::ControlModifier) {
        qint32 d = event->angleDelta().y();
        if(!checkScale(d)) {
            return;
        }
        qreal zoomFactor;
        QPointF oldPos = mapToScene(event->pos());
        if(d > 0) {
            zoomFactor = 1.3;
        } else {
            zoomFactor = 1 / 1.3;
        }
        scale(zoomFactor, zoomFactor);
        QPointF newPos = mapToScene(event->pos());
        QPointF delta = newPos - oldPos;
        translate(delta.x(), delta.y());
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

/**
 * @brief Check scale borders
 * @param delta : <0 - zoom out, >0 - zoom in
 * @return
 */
bool Chart::checkScale(qint32 delta) {
    QRectF vr = mapToScene(viewport()->geometry()).boundingRect();
    if(delta > 0 && (vr.right() - vr.left()) < 100) {
        return false;
    }
    if(delta < 0 && (vr.right() - vr.left()) > sceneRect().width() * 0.3) {
        return false;
    }
    return true;
}

/**
 * @brief Return current scene cursor position
 * @return
 */
QPointF Chart::getScenePos() {
    return scenePos;
}
