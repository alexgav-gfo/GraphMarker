#include "chartwidget.h"

/**
 * @brief ChartWidget constructor
 * @param parent
 */
ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent) {
    scene = new QGraphicsScene(0, 0, 100, 100);
    chart = new Chart(scene, this);

    initActions();
    initPopupMenu();

    layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(chart);
    setLayout(layout);
}

/**
 * @brief Init actions
 */
void ChartWidget::initActions() {
    actStartMark = new QAction(QIcon(":/icons/target_green.png"), "Start Mark");
    connect(actStartMark, &QAction::triggered, this, &ChartWidget::slotStartMark);
    actEndMark = new QAction(QIcon(":/icons/target_blue.png"), "End Mark");
    connect(actEndMark, &QAction::triggered, this, &ChartWidget::slotEndMark);
    actTopBorder = new QAction(QIcon(":/icons/line_green.png"), "Top Border");
    connect(actTopBorder, &QAction::triggered, this, &ChartWidget::slotTopBorder);
    actBottomBorder = new QAction(QIcon(":/icons/line_blue.png"), "Bottom Border");
    connect(actBottomBorder, &QAction::triggered, this, &ChartWidget::slotBottomBorder);
}

/**
 * @brief Init popup menu
 */
void ChartWidget::initPopupMenu() {
    popupMenu = new QMenu(this);
    popupMenu->setFont(QFont("sans-serif", 12));
    popupMenu->addAction(actStartMark);
    popupMenu->addAction(actEndMark);
    popupMenu->addAction(actTopBorder);
    popupMenu->addAction(actBottomBorder);
}

/**
 * @brief Load chart image and put it to graphics scene
 * @param path
 */
void ChartWidget::loadImage(const QString &path) {
    QImage img(path);
    if(!img.isNull()) {
        chart->resetMatrix();
        scene->clear();
        scene->setSceneRect(0, 0, img.width(), img.height());
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
        scene->addItem(item);
    }
}

/**
 * @brief Get chart items from scene as hash map
 * @return
 */
QMap<QString, Marker> ChartWidget::getItems() {
    return items;
}

/**
 * @brief Set items on scene
 * @param it
 */
void ChartWidget::setItems(const QMap<QString, Marker> &it) {
    auto i = items.constBegin();
    while(i != items.constEnd()) {
        scene->removeItem(i.value().item);
        ++i;
    }
    chart->update();
    update();
    items.clear();
    this->items = it;
    if(it.contains("start_mark")) {
        Marker m = it.value("start_mark");
        QPixmap p(":/icons/target_green.png");
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(p);
        qint32 x = m.coord.x() - p.width() / 2;
        qint32 y = m.coord.y() - p.height() / 2;
        item->setPos(x, y);
        scene->addItem(item);
        items["start_mark"].item = item;
    }
    if(it.contains("end_mark")) {
        Marker m = it.value("end_mark");
        QPixmap p(":/icons/target_blue.png");
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(p);
        qint32 x = m.coord.x() - p.width() / 2;
        qint32 y = m.coord.y() - p.height() / 2;
        item->setPos(x, y);
        scene->addItem(item);
        items["end_mark"].item = item;
    }
    if(it.contains("top_border")) {
        Marker m = it.value("top_border");
        QGraphicsLineItem *item = new QGraphicsLineItem(0, m.coord.y(), scene->width() - 1, m.coord.y());
        QPen pen(QBrush(Qt::darkGreen), 10, Qt::DashLine);
        item->setPen(pen);
        scene->addItem(item);
        items["top_border"].item = item;
    }
    if(it.contains("bottom_border")) {
        Marker m = it.value("bottom_border");
        QGraphicsLineItem *item = new QGraphicsLineItem(0, m.coord.y(), scene->width() - 1, m.coord.y());
        QPen pen(QBrush(Qt::blue), 10, Qt::DashLine);
        item->setPen(pen);
        scene->addItem(item);
        items["bottom_border"].item = item;
    }
}

/**
 * @brief Zoom chart
 * @param direction - in, out, original
 */
void ChartWidget::zoom(const ZoomDirect direction) {
    qint32 d = 0;
    if(direction == ZoomDirect::ZOOM_IN) {
        d = 1;
    } else if(direction == ZoomDirect::ZOOM_OUT) {
        d = -1;
    }
    if(!chart->checkScale(d)) {
        return;
    }
    qreal scale;
    if(direction == ZoomDirect::ZOOM_IN) {
        scale = SCALE_FACTOR;
    } else if(direction == ZoomDirect::ZOOM_OUT) {
        scale = 1 / SCALE_FACTOR;
    } else {
        scale = 1; // Make compiler happy
    }
    QPointF viewCenterOld = chart->mapToScene(chart->viewport()->geometry()).boundingRect().center();
    if(direction == ZoomDirect::ZOOM_ORIG) {
        chart->resetMatrix();
    } else {
        chart->scale(scale, scale);
    }
    QPointF viewCenterNew = chart->mapToScene(chart->viewport()->geometry()).boundingRect().center();
    QPointF delta = viewCenterNew - viewCenterOld;
    chart->translate(delta.x(), delta.y());
}

/**
 * @brief Zoom In
 */
void ChartWidget::zoomIn() {
    zoom(ZoomDirect::ZOOM_IN);
}

/**
 * @brief Zoom Out
 */
void ChartWidget::zoomOut() {
    zoom(ZoomDirect::ZOOM_OUT);
}

/**
 * @brief Zoom Original
 */
void ChartWidget::zoomOrig() {
    zoom(ZoomDirect::ZOOM_ORIG);
}

/**
 * @brief Mouse press event (for popup menu
 * @param event
 */
void ChartWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        QPoint pos = mapToGlobal(event->pos());
        pos.setX(pos.x() + 10);
        pos.setY(pos.y() + 10);
        popupMenu->exec(pos);
    } else {
        QWidget::mousePressEvent(event);
    }
}

/**
 * @brief Display question dialog
 * @param text
 * @return
 */
bool ChartWidget::question(const QString &text) {
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    QPoint cursorPos = QCursor::pos();
    msgBox.move(cursorPos.x() - 100, cursorPos.y() - 50);
    qint32 res = msgBox.exec();
    if(res == QMessageBox::Yes) {
        return true;
    }
    return false;
}

/**
 * @brief Reset scene
 */
void ChartWidget::reset() {
    scene->clear();
    items.clear();
}

/**
 * @brief Place start mark item
 */
void ChartWidget::slotStartMark() {
    const QString mark = "start_mark";
    QPointF scenePos = chart->getScenePos();
    QPixmap p(":/icons/target_green.png");
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(p);
    qint32 x = scenePos.x() - p.width() / 2;
    qint32 y = scenePos.y() - p.height() / 2;
    item->setPos(x, y);
    Marker m;
    m.coord = scenePos;
    m.item = item;
    if(items.contains(mark)) {
        if(question("Replace start timemark?")) {
            scene->removeItem(items.value(mark).item);
            scene->addItem(item);
            items.insert(mark, m);
        }
    } else {
        scene->addItem(item);
        items.insert(mark, m);
    }
    emit stateChanged();
}

/**
 * @brief Place end mark item
 */
void ChartWidget::slotEndMark() {
    const QString mark = "end_mark";
    QPointF scenePos = chart->getScenePos();
    QPixmap p(":/icons/target_blue.png");
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(p);
    qint32 x = scenePos.x() - p.width() / 2;
    qint32 y = scenePos.y() - p.height() / 2;
    item->setPos(x, y);
    Marker m;
    m.coord = scenePos;
    m.item = item;
    if(items.contains(mark)) {
        if(question("Replace end timemark?")) {
            scene->removeItem(items.value(mark).item);
            scene->addItem(item);
            items.insert(mark, m);
        }
    } else {
        scene->addItem(item);
        items.insert(mark, m);
    }
    emit stateChanged();
}

/**
 * @brief Place top border item
 */
void ChartWidget::slotTopBorder() {
    const QString mark = "top_border";
    QPointF scenePos = chart->getScenePos();
    QGraphicsLineItem *item = new QGraphicsLineItem(0, scenePos.y(), scene->width() - 1, scenePos.y());
    QPen pen(QBrush(Qt::darkGreen), 10, Qt::DashLine);
    item->setPen(pen);
    Marker m;
    m.coord = scenePos;
    m.item = item;
    if(items.contains(mark)) {
        if(question("Replace top border line?")) {
            scene->removeItem(items.value(mark).item);
            scene->addItem(item);
            items.insert(mark, m);
        }
    } else {
        scene->addItem(item);
        items.insert(mark, m);
    }
    emit stateChanged();
}

/**
 * @brief Place bottom border item
 */
void ChartWidget::slotBottomBorder() {
    const QString mark = "bottom_border";
    QPointF scenePos = chart->getScenePos();
    QGraphicsLineItem *item = new QGraphicsLineItem(0, scenePos.y(), scene->width() - 1, scenePos.y());
    QPen pen(QBrush(Qt::blue), 10, Qt::DashLine);
    item->setPen(pen);
    Marker m;
    m.coord = scenePos;
    m.item = item;
    if(items.contains(mark)) {
        if(question("Replace bottom border line?")) {
            scene->removeItem(items.value(mark).item);
            scene->addItem(item);
            items.insert(mark, m);
        }
    } else {
        scene->addItem(item);
        items.insert(mark, m);
    }
    emit stateChanged();
}
