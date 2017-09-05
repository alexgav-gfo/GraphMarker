#include "chartpropertyworker.h"

ChartPropertyWorker::ChartPropertyWorker() {
}

/**
 * @brief Read configuration from ini file as ChartProperty
 * @param file - file to read (full path)
 * @param chart - chart section name
 * @return
 */
ChartProperty ChartPropertyWorker::read(const QString &file, const QString &chart) {
    ChartProperty prop;
    prop.chart = chart;
    qint32 lastSlashIndex = file.lastIndexOf("/");
    prop.path = file.mid(0, lastSlashIndex);
    prop.fileName = file.mid(lastSlashIndex + 1, file.size());
    QFile f(file);
    if(!f.exists()) {
        prop.name = makeChartName(prop.fileName, chart);
        prop.err = true;
        return prop;
    }

    QSettings settings(file, QSettings::IniFormat);
    settings.beginGroup(chart);

    prop.err = false;
    QString name = settings.value("name", "").toString();
    if(name.isEmpty()) {
        prop.name = makeChartName(prop.fileName, chart);
    } else {
        prop.name = name;
    }
    prop.direction = settings.value("direction", "").toString().toLower();

    prop.beginTimemarkCoord = parseCoord(settings.value("start_timemark_coord").toString());
    if(prop.beginTimemarkCoord.isNull()) {
        prop.err = true;
        return prop;
    }
    prop.endTimemarkCoord = parseCoord(settings.value("end_timemark_coord").toString());
    if(prop.endTimemarkCoord.isNull()) {
        prop.err = true;
        return prop;
    }

    QString timemarkValue = settings.value("start_timemark_time", "").toString();
    if(timemarkValue.isEmpty()) {
        prop.beginTimemarkTime = QDateTime();
    } else {
        prop.beginTimemarkTime = QDateTime::fromString(timemarkValue, TIME_FMT);
    }
    timemarkValue = settings.value("end_timemark_time", "").toString();
    if(timemarkValue.isEmpty()) {
        prop.endTimemarkTime = QDateTime();
    } else {
        prop.endTimemarkTime = QDateTime::fromString(timemarkValue, TIME_FMT);
    }

    bool ok;
    qint32 val = settings.value("top_border_coord", SKIP_VAL).toInt(&ok);
    ok ? prop.topBorderCoord = val : prop.topBorderCoord = SKIP_VAL;
    val = settings.value("bottom_border_coord", SKIP_VAL).toInt(&ok);
    ok ? prop.bottomBorderCoord = val : prop.bottomBorderCoord = SKIP_VAL;

    return prop;
}

/**
 * @brief Write chart property to file
 * @param file - file to write (full path)
 * @param chart - chart section name
 * @param prop - ChartProperty object to write
 * @return
 */
SaveErr ChartPropertyWorker::write(const QString &file, const QString &chart, const ChartProperty &prop) {
    if(prop.direction.isEmpty()) {
        return SaveErr::ERR_DIRECTION;
    }
    if(prop.beginTimemarkCoord.isNull()) {
        return SaveErr::ERR_BEG_TM_COORD;
    }
    if(prop.beginTimemarkTime.isNull()) {
        return SaveErr::ERR_BEG_TM_TIME;
    }
    if(prop.endTimemarkCoord.isNull()) {
        return SaveErr::ERR_END_TM_COORD;
    }
    if(prop.endTimemarkTime.isNull()) {
        return SaveErr::ERR_END_TM_TIME;
    }
    QSettings settings(file, QSettings::IniFormat);
    if(!settings.isWritable()) {
        return SaveErr::ERR_NOT_WRITABLE;
    }
    settings.beginGroup(chart);
    settings.setValue("name", prop.name);
    settings.setValue("direction", prop.direction.toLower());
    QString coord = QString::number(prop.beginTimemarkCoord.x(), 'f', 0) + ":" + QString::number(prop.beginTimemarkCoord.y(), 'f', 0);
    settings.setValue("start_timemark_coord", coord);
    settings.setValue("start_timemark_time", prop.beginTimemarkTime.toString(TIME_FMT));
    coord = QString::number(prop.endTimemarkCoord.x(), 'f', 0) + ":" + QString::number(prop.endTimemarkCoord.y(), 'f', 0);
    settings.setValue("end_timemark_coord", coord);
    settings.setValue("end_timemark_time", prop.endTimemarkTime.toString(TIME_FMT));
    prop.topBorderCoord == SKIP_VAL ? settings.remove("top_border_coord") : settings.setValue("top_border_coord", prop.topBorderCoord);
    prop.bottomBorderCoord == SKIP_VAL ? settings.remove("bottom_border_coord") : settings.setValue("bottom_border_coord", prop.bottomBorderCoord);
    return SaveErr::SUCCESS;
}

/**
 * @brief Remove chart section completely
 * @param file - file to remove (full path)
 * @param section - section name
 * @return
 */
SaveErr ChartPropertyWorker::removeSection(const QString &file, const QString &section) {
    QSettings settings(file, QSettings::IniFormat);
    if(!settings.isWritable()) {
        return SaveErr::ERR_NOT_WRITABLE;
    }
    settings.remove(section);
    return SaveErr::SUCCESS;
}

/**
 * @brief Make chart name from file name ('YYYY_DOY_dp_ez_jz.png', extract 'dp', 'ez' or 'jz')
 * @param file
 * @param chart
 * @return
 */
QString ChartPropertyWorker::makeChartName(const QString &file, const QString &chart) {
    qint32 b1 = -1, b2 = -1, b3 = -1;
    if(file.contains(INI_PATTERN_DAYOFYEAR)) {
        b1 = 9;
        b2 = 12;
        b3 = 15;
    }
    if(file.contains(INI_PATTERN_DAYABS) || file.contains(INI_PATTERN_DAYABS_NUM)) {
        b1 = 10;
        b2 = 13;
        b3 = 16;
    }
    QString ch = "";
    if(chart == "chart1" && b1 > 0) {
        ch = file.mid(b1, 2);
    }
    if(chart == "chart2" && b2 > 0) {
        ch = file.mid(b2, 2);
    }
    if(chart == "chart3" && b3 > 0) {
        ch = file.mid(b3, 2);
    }
    return ch;
}

/**
 * @brief Parse coord string ('XXX:YYY')
 * @param coordStr
 * @return
 */
QPoint ChartPropertyWorker::parseCoord(const QString &coordStr) {
    QStringList l = coordStr.split(":");
    if(l.size() != 2) {
        return QPoint();
    }
    bool ok1, ok2;
    qint32 x = l.at(0).toInt(&ok1);
    qint32 y = l.at(1).toInt(&ok2);
    if(ok1 && ok2) {
        return QPoint(x, y);
    } else {
        return QPoint();
    }
}
