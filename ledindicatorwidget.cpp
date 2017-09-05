#include "ledindicatorwidget.h"

/**
 * @brief LedIndicatorWidget constructor
 * @param parent
 */
LedIndicatorWidget::LedIndicatorWidget(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(16, 16);
    state = false;
}

/**
 * @brief Set widget ON / OFF (green / red)
 * @param enabled - true - ON, false - OFF
 */
void LedIndicatorWidget::setOn(bool enabled) {
    state = enabled;
    repaint();
}

/**
 * @brief Paint event
 */
void LedIndicatorWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    const qint32 W = this->width();
    const qint32 H = this->height();
    const QColor onColor(0, 153, 0); // Green
    const QColor offColor(153, 0, 0); // Red
    if(state) {
        p.fillRect(0, 0, W, H, onColor);
    } else {
        p.fillRect(0, 0, W, H, offColor);
    }
    p.setPen(QPen(Qt::black, 2));
    p.drawRect(0, 0, W, H);
}
