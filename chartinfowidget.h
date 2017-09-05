#ifndef CHARTINFOWIDGET_H
#define CHARTINFOWIDGET_H

#include <QFrame>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ledindicatorwidget.h"
#include "chartpropertyworker.h"
#include "dateselectdialog.h"

const QString DIRECTION_LBL = "Direction: ";
const QString BEG_MARK_TIME_LBL = "Start Time: ";
const QString END_MARK_TIME_LBL = "End Time: ";
const QString START_MARK_LBL = "Start Mark: ";
const QString END_MARK_LBL = "End Mark: ";
const QString TOP_BORDER_MARK = "Top Border: ";
const QString BOT_BORDER_MARK = "Bottom Border: ";

class ChartInfoWidget : public QFrame {
    Q_OBJECT
public:
    explicit ChartInfoWidget(QFrame *parent = 0);
    void setWidgetEnabled(bool enabled);
    void setProperty(const ChartProperty &prop);
    ChartProperty getProperty();
    bool isChanged();
    void setChanged(bool changed);
    void reset();

signals:
    void stateChanged();

public slots:

protected:

private:
    void init();
    QString makeTimemarkText(const QString &header, const QPointF &p);
    QDateTime makeDateTime(const QString &fileName);

    ChartProperty property;

    QButtonGroup *directionButtonGroup;
    QRadioButton *directionUp;
    QRadioButton *directionDown;
    LedIndicatorWidget *directionLed;
    QLabel *titleLbl;
    QLabel *beginDateTime;
    QPushButton *beginDateTimeSetBtn;
    QPushButton *beginDateTimeClrBtn;
    LedIndicatorWidget *beginDateTimeLed;
    QLabel *endDateTime;
    QPushButton *endDateTimeSetBtn;
    QPushButton *endDateTimeClrBtn;
    LedIndicatorWidget *endDateTimeLed;
    QLabel *beginTimeMark;
    QPushButton *beginTimeMarkBtn;
    LedIndicatorWidget *beginTimeMarkLed;
    QLabel *endTimeMark;
    QPushButton *endTimeMarkBtn;
    LedIndicatorWidget *endTimeMarkLed;
    QLabel *beginDivisor;
    QPushButton *beginDivisorBtn;
    LedIndicatorWidget *beginDivisorLed;
    QLabel *endDivisor;
    QPushButton *endDivisorBtn;
    LedIndicatorWidget *endDivisorLed;
    QPushButton *resetBtn;
    QPushButton *removeBtn;

    bool changed;

    const QString TIME_FMT = "yyyy-MM-dd HH:mm";

private slots:
    void slotSetBeginDateTime();
    void slotClearBeginDateTime();
    void slotSetEndDateTime();
    void slotClearEndDateTime();
    void slotClearBeginTimeMark();
    void slotClearEndTimeMark();
    void slotClearBeginDivisor();
    void slotClearEndDivisor();
    void slotDirection();
    void slotReset();
    void slotRemoveConfig();
};

#endif // CHARTINFOWIDGET_H
