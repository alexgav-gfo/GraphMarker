#ifndef CHARTPROPERTYWORKER_H
#define CHARTPROPERTYWORKER_H

#include <QFile>
#include <QSettings>
#include <QPoint>
#include <QDateTime>
#include <QDebug>
#include "types.h"

static const qint32 SKIP_VAL = 99999;

enum class SaveErr {
    SUCCESS,
    ERR_NOT_WRITABLE,
    ERR_BEG_TM_TIME,
    ERR_END_TM_TIME,
    ERR_BEG_TM_COORD,
    ERR_END_TM_COORD,
    ERR_DIRECTION
};

typedef struct {
    QString chart;
    QString name;
    QString path;
    QString fileName;
    QString direction;
    QPointF beginTimemarkCoord;
    QDateTime beginTimemarkTime;
    QPointF endTimemarkCoord;
    QDateTime endTimemarkTime;
    qint32 topBorderCoord = SKIP_VAL;
    qint32 bottomBorderCoord = SKIP_VAL;
    bool err;
} ChartProperty;

class ChartPropertyWorker {
public:
    ChartPropertyWorker();
    ChartProperty read(const QString &file, const QString &chart);
    SaveErr write(const QString &file, const QString &chart, const ChartProperty &prop);
    SaveErr removeSection(const QString &file, const QString &section);

private:
    QPoint parseCoord(const QString &coordStr);
    QString makeChartName(const QString &file, const QString &chart);

    const QString TIME_FMT = "yyyy-MM-dd:HH";
};

#endif // CHARTPROPERTYWORKER_H
