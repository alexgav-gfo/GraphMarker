#include "chartinfowidget.h"

/**
 * @brief ChartInfoWidget - constructor
 * @param parent
 */
ChartInfoWidget::ChartInfoWidget(QFrame *parent) : QFrame(parent) {
    setFixedWidth(300);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    init();
    changed = false;
}

/**
 * @brief Init widget
 */
void ChartInfoWidget::init() {
    const qint32 BTN_W = 24;
    const qint32 BTN_H = 20;

    QFont titleFont("sans-serif", 12, QFont::Bold);
    QFont timeFont("sans-serif", 11, QFont::Normal);
    QFont labelFont("sans-serif", 11);

    titleLbl = new QLabel(tr("Chart"));
    titleLbl->setFont(titleFont);
    titleLbl->setAlignment(Qt::AlignCenter);

    QHBoxLayout *bdtLayout = new QHBoxLayout();

    beginDateTimeLed = new LedIndicatorWidget();
    bdtLayout->addWidget(beginDateTimeLed);
    beginDateTime = new QLabel(BEG_MARK_TIME_LBL);
    beginDateTime->setFont(timeFont);
    bdtLayout->addWidget(beginDateTime);
    beginDateTimeSetBtn = new QPushButton("...");
    beginDateTimeSetBtn->setFixedSize(BTN_W, BTN_H);
    connect(beginDateTimeSetBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotSetBeginDateTime);
    beginDateTimeClrBtn = new QPushButton("Clr");
    beginDateTimeClrBtn->setFixedSize(BTN_W, BTN_H);
    connect(beginDateTimeClrBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearBeginDateTime);
    bdtLayout->addWidget(beginDateTimeSetBtn);
    bdtLayout->addWidget(beginDateTimeClrBtn);

    QHBoxLayout *edtLayout = new QHBoxLayout();
    endDateTimeLed = new LedIndicatorWidget();
    edtLayout->addWidget(endDateTimeLed);
    endDateTime = new QLabel(END_MARK_TIME_LBL);
    endDateTime->setFont(timeFont);
    edtLayout->addWidget(endDateTime);
    endDateTimeSetBtn = new QPushButton("...");
    endDateTimeSetBtn->setFixedSize(BTN_W, BTN_H);
    connect(endDateTimeSetBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotSetEndDateTime);
    endDateTimeClrBtn = new QPushButton("Clr");
    endDateTimeClrBtn->setFixedSize(BTN_W, BTN_H);
    connect(endDateTimeClrBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearEndDateTime);
    edtLayout->addWidget(endDateTimeSetBtn);
    edtLayout->addWidget(endDateTimeClrBtn);

    QHBoxLayout *btmLayout = new QHBoxLayout();
    beginTimeMarkLed = new LedIndicatorWidget();
    btmLayout->addWidget(beginTimeMarkLed);
    beginTimeMark = new QLabel(START_MARK_LBL);
    beginTimeMark->setFont(labelFont);
    btmLayout->addWidget(beginTimeMark);
    beginTimeMarkBtn = new QPushButton("Clr");
    beginTimeMarkBtn->setFixedSize(BTN_W, BTN_H);
    connect(beginTimeMarkBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearBeginTimeMark);
    btmLayout->addWidget(beginTimeMarkBtn);

    QHBoxLayout *etmLayout = new QHBoxLayout();
    endTimeMarkLed = new LedIndicatorWidget();
    etmLayout->addWidget(endTimeMarkLed);
    endTimeMark = new QLabel(END_MARK_LBL);
    endTimeMark->setFont(labelFont);
    etmLayout->addWidget(endTimeMark);
    endTimeMarkBtn = new QPushButton("Clr");
    endTimeMarkBtn->setFixedSize(BTN_W, BTN_H);
    connect(endTimeMarkBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearEndTimeMark);
    etmLayout->addWidget(endTimeMarkBtn);

    QHBoxLayout *bdivLayout = new QHBoxLayout();
    beginDivisorLed = new LedIndicatorWidget();
    bdivLayout->addWidget(beginDivisorLed);
    beginDivisor = new QLabel(TOP_BORDER_MARK);
    beginDivisor->setFont(labelFont);
    bdivLayout->addWidget(beginDivisor);
    beginDivisorBtn = new QPushButton("Clr");
    beginDivisorBtn->setFixedSize(BTN_W, BTN_H);
    connect(beginDivisorBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearBeginDivisor);
    bdivLayout->addWidget(beginDivisorBtn);

    QHBoxLayout *edivLayout = new QHBoxLayout();
    endDivisorLed = new LedIndicatorWidget();
    edivLayout->addWidget(endDivisorLed);
    endDivisor = new QLabel(BOT_BORDER_MARK);
    endDivisor->setFont(labelFont);
    edivLayout->addWidget(endDivisor);
    endDivisorBtn = new QPushButton("Clr");
    endDivisorBtn->setFixedSize(BTN_W, BTN_H);
    connect(endDivisorBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotClearEndDivisor);
    edivLayout->addWidget(endDivisorBtn);

    QHBoxLayout *dirLayout = new QHBoxLayout;
    directionLed = new LedIndicatorWidget;
    directionLed->setOn(false);
    dirLayout->addWidget(directionLed);
    QLabel *dirLabel = new QLabel(DIRECTION_LBL);
    dirLabel->setFont(labelFont);
    dirLayout->addWidget(dirLabel);
    directionButtonGroup = new QButtonGroup;
    directionUp = new QRadioButton(tr("UP"));
    directionUp->setFont(labelFont);
    directionUp->setChecked(false);
    directionButtonGroup->addButton(directionUp);
    dirLayout->addWidget(directionUp);
    directionDown = new QRadioButton(tr("DOWN"));
    directionDown->setFont(labelFont);
    directionDown->setChecked(false);
    directionButtonGroup->addButton(directionDown);

    dirLayout->addWidget(directionDown);
    dirLayout->addStretch();

    connect(directionUp, &QRadioButton::toggled, this, &ChartInfoWidget::slotDirection);
    connect(directionDown, &QRadioButton::toggled, this, &ChartInfoWidget::slotDirection);

    QHBoxLayout *resetLayout = new QHBoxLayout;
    resetBtn = new QPushButton(tr("&Reset"));
    connect(resetBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotReset);
    resetLayout->addWidget(resetBtn);
    removeBtn = new QPushButton(tr("Remove &Config"));
    connect(removeBtn, &QPushButton::clicked, this, &ChartInfoWidget::slotRemoveConfig);
    resetLayout->addWidget(removeBtn);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(titleLbl);
    layout->addLayout(dirLayout);
    layout->addLayout(bdtLayout);
    layout->addLayout(edtLayout);
    layout->addLayout(btmLayout);
    layout->addLayout(etmLayout);
    layout->addLayout(bdivLayout);
    layout->addLayout(edivLayout);
    layout->addLayout(resetLayout);
    this->setLayout(layout);
    adjustSize();
}

/**
 * @brief Set widget information from ChartProperty
 * @param prop
 */
void ChartInfoWidget::setProperty(const ChartProperty &prop) {
    this->property = prop;
    titleLbl->setText("Chart: " + prop.name.toUpper());
    if(prop.direction.isEmpty()) {
        directionUp->setChecked(false);
        directionDown->setChecked(false);
        directionLed->setOn(false);
    } else if(prop.direction.toLower() == "up") {
        directionUp->setChecked(true);
        directionDown->setChecked(false);
        directionLed->setOn(true);
    } else {
        directionUp->setChecked(false);
        directionDown->setChecked(true);
        directionLed->setOn(true);
    }
    if(!prop.beginTimemarkTime.isNull()) {
        beginDateTime->setText(BEG_MARK_TIME_LBL + prop.beginTimemarkTime.toString(TIME_FMT));
        beginDateTimeLed->setOn(true);
    } else {
        beginDateTime->setText(BEG_MARK_TIME_LBL);
        beginDateTimeLed->setOn(false);
    }
    if(!prop.endTimemarkTime.isNull()) {
        endDateTime->setText(END_MARK_TIME_LBL + prop.endTimemarkTime.toString(TIME_FMT));
        endDateTimeLed->setOn(true);
    } else {
        endDateTime->setText(END_MARK_TIME_LBL);
        endDateTimeLed->setOn(false);
    }
    if(!prop.beginTimemarkCoord.isNull()) {
        beginTimeMark->setText(makeTimemarkText(START_MARK_LBL, prop.beginTimemarkCoord));
        beginTimeMarkLed->setOn(true);
    } else {
        beginTimeMark->setText(START_MARK_LBL);
        beginTimeMarkLed->setOn(false);
    }
    if(!prop.endTimemarkCoord.isNull()) {
        endTimeMark->setText(makeTimemarkText(END_MARK_LBL, prop.endTimemarkCoord));
        endTimeMarkLed->setOn(true);
    } else {
        endTimeMark->setText(END_MARK_LBL);
        endTimeMarkLed->setOn(false);
    }
    if(prop.topBorderCoord != SKIP_VAL) {
        beginDivisor->setText(TOP_BORDER_MARK + QString::number(prop.topBorderCoord, 'f', 0));
        beginDivisorLed->setOn(true);
    } else {
        beginDivisor->setText(TOP_BORDER_MARK);
        beginDivisorLed->setOn(false);
    }
    if(prop.bottomBorderCoord != SKIP_VAL) {
        endDivisor->setText(BOT_BORDER_MARK + QString::number(prop.bottomBorderCoord, 'f', 0));
        endDivisorLed->setOn(true);
    } else {
        endDivisor->setText(BOT_BORDER_MARK);
        endDivisorLed->setOn(false);
    }
    changed = false;
}

/**
 * @brief Get current widget property
 * @return
 */
ChartProperty ChartInfoWidget::getProperty() {
    if(directionUp->isChecked()) {
        property.direction = "up";
    } else if(directionDown->isChecked()) {
        property.direction = "down";
    } else {
        property.direction = "";
    }
    return this->property;
}

/**
 * @brief Make text for timemark coord label
 * @param header
 * @param p
 * @return
 */
QString ChartInfoWidget::makeTimemarkText(const QString &header, const QPointF &p) {
    return header + ": " + QString::number(p.x(), 'f', 0) + ":" + QString::number(p.y(), 'f', 0);
}

/**
 * @brief Make text for timemark time label
 * @param fileName
 * @return
 */
QDateTime ChartInfoWidget::makeDateTime(const QString &fileName) {
    bool ok1, ok2;
    QString yearStr = fileName.mid(0, 4);
    qint32 year = yearStr.toInt(&ok1);
    QString dayStr;
    qint32 day;
    if(fileName.contains(INI_PATTERN_DAYOFYEAR)) {
        dayStr = fileName.mid(5, 3);
        day = dayStr.toInt(&ok2);
        if(ok1 && ok2) {
            QDate d(year, 1, 1);
            d = d.addDays(day);
            return QDateTime(d, QTime(0, 0, 0));
        }
    }
    if(fileName.contains(INI_PATTERN_DAYABS) || fileName.contains(INI_PATTERN_DAYABS_NUM)) {
        dayStr = fileName.mid(5, 4);
        day = dayStr.toInt(&ok2);
        if(ok1 && ok2) {
            QDate d(1986, 2, 26); // Measuring start
            d = d.addDays(day);
            return QDateTime(d, QTime(0, 0, 0));
        }
    }
    return QDateTime::currentDateTime();
}

/**
 * @brief Set date & time for begin timemark
 */
void ChartInfoWidget::slotSetBeginDateTime() {
    DateSelectDialog dlg;
    if(!property.beginTimemarkTime.isNull()) {
        dlg.setTime(property.beginTimemarkTime);
    } else {
        dlg.setTime(makeDateTime(property.fileName));
    }
    QPoint pos = QCursor::pos();
    dlg.move(pos.x() - dlg.width() + 50, pos.y() + 10);
    qint32 ret = dlg.exec();
    if(ret == QDialog::Accepted) {
        QDateTime t = dlg.getTime();
        if(!t.isNull() && t.isValid()) {
            property.beginTimemarkTime = t;
            beginDateTime->setText(BEG_MARK_TIME_LBL + property.beginTimemarkTime.toString(TIME_FMT));
            beginDateTimeLed->setOn(true);
        } else {
            beginDateTime->clear();
            beginDateTimeLed->setOn(false);
        }
    }
    changed = true;
}

/**
 * @brief Clear date & time for begin timemark
 */
void ChartInfoWidget::slotClearBeginDateTime() {
    beginDateTime->setText(BEG_MARK_TIME_LBL);
    beginDateTimeLed->setOn(false);
    property.beginTimemarkTime = QDateTime();
    changed = true;

}

/**
 * @brief Set date & time for end timemark
 */
void ChartInfoWidget::slotSetEndDateTime() {
    DateSelectDialog dlg;
    if(!property.endTimemarkTime.isNull()) {
        dlg.setTime(property.endTimemarkTime);
    } else {
        dlg.setTime(makeDateTime(property.fileName));
    }
    QPoint pos = QCursor::pos();
    dlg.move(pos.x() - dlg.width() + 50, pos.y() + 10);
    qint32 ret = dlg.exec();
    if(ret == QDialog::Accepted) {
        QDateTime t = dlg.getTime();
        if(!t.isNull() && t.isValid()) {
            property.endTimemarkTime = t;
            endDateTime->setText(END_MARK_TIME_LBL + property.endTimemarkTime.toString(TIME_FMT));
            endDateTimeLed->setOn(true);
        } else {
            endDateTime->clear();
            endDateTimeLed->setOn(false);
        }
    }
    changed = true;
}

/**
 * @brief Clear date & time for end timemark
 */
void ChartInfoWidget::slotClearEndDateTime() {
    endDateTime->setText(END_MARK_TIME_LBL);
    endDateTimeLed->setOn(false);
    property.endTimemarkTime = QDateTime();
    changed = true;
}

/**
 * @brief Clear begin timemark coords
 */
void ChartInfoWidget::slotClearBeginTimeMark() {
    beginTimeMark->setText(START_MARK_LBL);
    beginTimeMarkLed->setOn(false);
    property.beginTimemarkCoord = QPoint();
    changed = true;
    emit stateChanged();
}

/**
 * @brief Clear end timemark coords
 */
void ChartInfoWidget::slotClearEndTimeMark() {
    endTimeMark->setText(END_MARK_LBL);
    endTimeMarkLed->setOn(false);
    property.endTimemarkCoord = QPoint();
    emit stateChanged();
}

/**
 * @brief Clear begin timemark divisor
 */
void ChartInfoWidget::slotClearBeginDivisor() {
    beginDivisor->setText(TOP_BORDER_MARK);
    beginDivisorLed->setOn(false);
    property.topBorderCoord = SKIP_VAL;
    changed = true;
    emit stateChanged();
}

/**
 * @brief Clear end timemark divisor
 */
void ChartInfoWidget::slotClearEndDivisor() {
    endDivisor->setText(BOT_BORDER_MARK);
    endDivisorLed->setOn(false);
    property.bottomBorderCoord = SKIP_VAL;
    changed = true;
    emit stateChanged();
}

/**
 * @brief Change Direction
 */
void ChartInfoWidget::slotDirection() {
    directionLed->setOn(true);
    changed = true;
    emit stateChanged();
}

/**
 * @brief Reset Info slot
 */
void ChartInfoWidget::slotReset() {
    QMessageBox box;
    box.setText("Confirm Reset");
    box.setInformativeText("Do you want to reset all changes ?");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::No);
    box.setIcon(QMessageBox::Question);
    qint32 res = box.exec();
    if(res == QMessageBox::Yes) {
        reset();
        emit stateChanged();
    }
}

/**
 * @brief Reset all information
 */
void ChartInfoWidget::reset() {
    beginDateTime->setText(BEG_MARK_TIME_LBL);
    beginDateTimeLed->setOn(false);
    property.beginTimemarkTime = QDateTime();
    endDateTime->setText(END_MARK_TIME_LBL);
    endDateTimeLed->setOn(false);
    property.endTimemarkTime = QDateTime();
    beginTimeMark->setText(START_MARK_LBL);
    beginTimeMarkLed->setOn(false);
    property.beginTimemarkCoord = QPoint();
    endTimeMark->setText(END_MARK_LBL);
    endTimeMarkLed->setOn(false);
    property.endTimemarkCoord = QPoint();
    beginDivisor->setText(TOP_BORDER_MARK);
    beginDivisorLed->setOn(false);
    property.topBorderCoord = SKIP_VAL;
    endDivisor->setText(BOT_BORDER_MARK);
    endDivisorLed->setOn(false);
    property.bottomBorderCoord = SKIP_VAL;
    directionButtonGroup->setExclusive(false);
    directionUp->setChecked(false);
    directionDown->setChecked(false);
    directionButtonGroup->setExclusive(true);
    directionLed->setOn(false);
    changed = false;
}

/**
 * @brief Remove config in ini file (entire section)
 */
void ChartInfoWidget::slotRemoveConfig() {
    qint32 res = QMessageBox::question(this, "Remove Config", "Do you really want to permanently delete chart config from file ?");
    if(res == QMessageBox::Yes) {
        ChartPropertyWorker cpw;
        QString file = property.path + "/" + property.fileName;
        SaveErr err = cpw.removeSection(file, property.chart);
        if(err == SaveErr::ERR_NOT_WRITABLE) {
            QMessageBox::warning(this, "Error", "Cannot delete chart config, file not writable.");
        }
        reset();
        changed = false;
        emit stateChanged();
    }
}

/**
 * @brief Set widget enabled
 * @param enabled
 */
void ChartInfoWidget::setWidgetEnabled(bool enabled) {
    this->setEnabled(enabled);
}

/**
 * @brief Return true if info is changed
 * @return
 */
bool ChartInfoWidget::isChanged() {
    return changed;
}

/**
 * @brief Set info changed flag
 * @param changed
 */
void ChartInfoWidget::setChanged(bool changed) {
    this->changed = changed;
}
