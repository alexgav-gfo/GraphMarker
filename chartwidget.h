#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>
#include "chart.h"

enum class ZoomDirect {
    ZOOM_IN,
    ZOOM_OUT,
    ZOOM_ORIG
};

typedef struct {
    QPointF coord;
    QGraphicsItem *item;
} Marker;

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = 0);
    void loadImage(const QString &path);
    QMap<QString, Marker> getItems();
    void setItems(const QMap<QString, Marker> &it);
    void zoomIn();
    void zoomOut();
    void zoomOrig();
    void reset();

signals:
    void stateChanged();

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void initActions();
    void initPopupMenu();
    bool question(const QString &text);
    void zoom(const ZoomDirect direction);

    QMap<QString, Marker> items;

    QAction *actStartMark;
    QAction *actEndMark;
    QAction *actTopBorder;
    QAction *actBottomBorder;

    QGridLayout *layout;
    QGraphicsScene *scene;
    Chart *chart;
    QMenu *popupMenu;

    const qreal SCALE_FACTOR = 1.3;

private slots:
    void slotStartMark();
    void slotEndMark();
    void slotTopBorder();
    void slotBottomBorder();
};

#endif // CHARTWIDGET_H
