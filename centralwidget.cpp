#include "centralwidget.h"

/**
 * @brief CentralWidget constructor
 * @param parent
 */
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
    // Setup chart widget
    chartWidget = new ChartWidget(this);
    connect(chartWidget, &ChartWidget::stateChanged, this, &CentralWidget::slotChartWidgetChanged);
    chartWidget->setEnabled(false);
    layout = new QHBoxLayout();
    layout->setContentsMargins(3, 3, 3, 3);
    layout->setSpacing(3);
    layout->addWidget(chartWidget);
    // Setup info widgets
    infoLayout = new QVBoxLayout();
    chartSelect = new ChartSelectWidget;
    connect(chartSelect, &ChartSelectWidget::selected, this, &CentralWidget::slotChartSelected);
    infoLayout->addWidget(chartSelect);
    infoWidget1 = new ChartInfoWidget();
    connect(infoWidget1, &ChartInfoWidget::stateChanged, this, &CentralWidget::slotInfoWidgetOneChanged);
    infoWidget1->setEnabled(false);
    infoLayout->addWidget(infoWidget1);
    infoWidget2 = new ChartInfoWidget();
    connect(infoWidget2, &ChartInfoWidget::stateChanged, this, &CentralWidget::slotInfoWidgetTwoChanged);
    infoWidget2->setEnabled(false);
    infoLayout->addWidget(infoWidget2);
    infoWidget3 = new ChartInfoWidget();
    connect(infoWidget3, &ChartInfoWidget::stateChanged, this, &CentralWidget::slotInfoWidgetThreeChanged);
    infoWidget3->setEnabled(false);
    infoLayout->addWidget(infoWidget3);
    infoLayout->addStretch();

    layout->addLayout(infoLayout);
    this->setLayout(layout);

    opened = false;
}

/**
 * @brief Open file action
 */
void CentralWidget::slotOpen() {
    Settings &set = Settings::instance();
    QString dir = set.getCurrentDir();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Chart Image"), dir, tr("Images (*.png)"));
    if(!fileName.isEmpty()) {
        if(!checkFileName(fileName)) {
            QMessageBox::critical(this, tr("File name error"), tr("File name not corresponds to pattern ('YYYY_DOY_xx_yy_zz.png'),\n"
                                                                  "   where YYYY - year, DOY - day of year,\n"
                                                                  "   xx - name of chart 1,\n"
                                                                  "   yy - name of chart 2,\n"
                                                                  "   zz - name of chart 3."));
            return;
        }
        openFile(fileName);
        currentFile = fileName;
        QString dirPath = currentFile.mid(0, currentFile.lastIndexOf('/'));
        QDir d(dirPath);
        currentDir.clear();
        currentDir << d.entryList(QDir::Files, QDir::Name);
        set.setCurrentDir(currentFile);
        emit fileOpened();
    }
}

/**
 * @brief Check if filename match with pattern (FILENAME_PATTERN)
 * @param fileName
 * @return true if match
 */
bool CentralWidget::checkFileName(const QString &fileName) {
    QString f = fileName.mid(fileName.lastIndexOf('/') + 1);
    if(f.contains(FILENAME_PATTERN_DAYOFYEAR) || f.contains(FILENAME_PATTERN_DAYABS) || f.contains(FILENAME_PATTERN_DAYABS_NUM)) {
        return true;
    }
    return false;
}

/**
 * @brief Open chart file
 * @param file - chart filename (full path)
 */
void CentralWidget::openFile(const QString &file) {
    chartWidget->reset();
    chartWidget->loadImage(file);
    chartWidget->setEnabled(true);
    propertyFile = file.mid(0, file.lastIndexOf('.')) + ".ini";
    ChartPropertyWorker cpw;
    ChartProperty prop = cpw.read(propertyFile, "chart1");
    infoWidget1->reset();
    infoWidget1->setProperty(prop);
    chartSelect->setChartOneName(prop.name);
    prop = cpw.read(propertyFile, "chart2");
    infoWidget2->reset();
    infoWidget2->setProperty(prop);
    chartSelect->setChartTwoName(prop.name);
    prop = cpw.read(propertyFile, "chart3");
    infoWidget3->reset();
    infoWidget3->setProperty(prop);
    chartSelect->setChartThreeName(prop.name);
    opened = true;
    slotChartSelected();
}

/**
 * @brief Close file
 * @return false - User press cancel or some save error occured
 */
bool CentralWidget::closeFile() {
    SaveState state = saveAllCharts();
    if(state == SaveState::BTN_CANCEL || state == SaveState::NOT_OPENED) {
        return false;
    }
    if(opened && (state == SaveState::SAVE_SUCCESS || state == SaveState::NOT_CHANGED || state == SaveState::BTN_NO)) {
        chartWidget->reset();
        chartWidget->setEnabled(false);
        infoWidget1->reset();
        infoWidget1->setEnabled(false);
        infoWidget2->reset();
        infoWidget2->setEnabled(false);
        infoWidget3->reset();
        infoWidget3->setEnabled(false);
        currentFile = QString();
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Close file action
 */
void CentralWidget::slotClose() {
    if(closeFile()) {
        opened = false;
        emit fileClosed();
    }
}

/**
 * @brief Next file action
 */
void CentralWidget::slotNext() {
    nextFile(true);
}

/**
 * @brief Previous file action
 */
void CentralWidget::slotPrev() {
    nextFile(false);
}

/**
 * @brief Get next file
 * @param direction - true: next, false: previous
 * @return
 */
void CentralWidget::nextFile(bool direction) {
    if(currentFile.isEmpty()) {
        return;
    }
    qint32 lastSlashInd = currentFile.lastIndexOf('/');
    QString dir = currentFile.mid(0, lastSlashInd);
    QString file = currentFile.mid(lastSlashInd + 1);
    qint32 pos = 0;
    for(qint32 i = 0; i < currentDir.length(); ++i) {
        if(currentDir.at(i) == file) {
            pos = i;
            break;
        }
    }
    direction ? pos++ : pos--;
    if(pos >= 0 && pos < currentDir.size()) {
        QString nextFile = dir + "/" + currentDir.at(pos);
        if(!closeFile()) {
            return;
        }
        openFile(nextFile);
        currentFile = nextFile;
        emit fileOpened();
    }
}

/**
 * @brief Zoom In action
 */
void CentralWidget::slotZoomIn() {
    chartWidget->zoomIn();
}

/**
 * @brief Zoom Out action
 */
void CentralWidget::slotZoomOut() {
    chartWidget->zoomOut();
}

/**
 * @brief Zoom Original action
 */
void CentralWidget::slotZoomOriginal() {
    chartWidget->zoomOrig();
}

/**
 * @brief Construct chart items map from ChartProperty
 * @param prop
 * @return
 */
QMap<QString, Marker> CentralWidget::itemsFromProperty(const ChartProperty &prop) {
    QMap<QString, Marker> res;
    Marker m;
    m.item = nullptr;
    if(!prop.beginTimemarkCoord.isNull()) {
        m.coord = prop.beginTimemarkCoord;
        res.insert("start_mark", m);
    }
    if(!prop.endTimemarkCoord.isNull()) {
        m.coord = prop.endTimemarkCoord;
        res.insert("end_mark", m);
    }
    if(prop.topBorderCoord != SKIP_VAL) {
        m.coord = QPointF(0, prop.topBorderCoord);
        res.insert("top_border", m);
    }
    if(prop.bottomBorderCoord != SKIP_VAL) {
        m.coord = QPointF(0, prop.bottomBorderCoord);
        res.insert("bottom_border", m);
    }
    return res;
}

/**
 * @brief Chart selected slot (handle signal from ChartSelectWidget)
 */
void CentralWidget::slotChartSelected() {
    if(!opened) {
        return;
    }
    switch(chartSelect->getChart()) {
    case Charts::CHART_ONE:
        infoWidget1->setWidgetEnabled(true);
        infoWidget2->setWidgetEnabled(false);
        infoWidget3->setWidgetEnabled(false);
        slotInfoWidgetOneChanged();
        break;
    case Charts::CHART_TWO:
        infoWidget1->setWidgetEnabled(false);
        infoWidget2->setWidgetEnabled(true);
        infoWidget3->setWidgetEnabled(false);
        slotInfoWidgetTwoChanged();
        break;
    case Charts::CHART_THREE:
        infoWidget1->setWidgetEnabled(false);
        infoWidget2->setWidgetEnabled(false);
        infoWidget3->setWidgetEnabled(true);
        slotInfoWidgetThreeChanged();
        break;
    }
}

/**
 * @brief Confirm Save question dialog box
 * @return
 */
qint32 CentralWidget::confirmSaveQuestion() {
    QMessageBox box;
    box.setText("Confirm Save");
    box.setInformativeText("Do you want to save changes?");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Yes);
    box.setIcon(QMessageBox::Question);
    return box.exec();
}

/**
 * @brief Save chart
 * @param section - section in ini file ([chartX])
 * @param title - Title for error window
 * @param info - pointer for chart info widget
 * @return
 */
SaveState CentralWidget::saveChart(const QString &section, const QString &title, ChartInfoWidget *info) {
    if(!info->isChanged()) {
        return SaveState::SAVE_SUCCESS;
    }
    ChartPropertyWorker cpw;
    SaveErr res = cpw.write(propertyFile, section, info->getProperty());
    if(res != SaveErr::SUCCESS) {
        displaySaveError(title, res);
        return SaveState::SAVE_FAIL;
    } else {
        info->setChanged(false);
        return SaveState::SAVE_SUCCESS;
    }
}

/**
 * @brief Save all charts
 * @return
 */
SaveState CentralWidget::saveAllCharts() {
    if(!opened) {
        return SaveState::NOT_OPENED;
    }
    if(infoWidget1->isChanged() || infoWidget2->isChanged() || infoWidget3->isChanged()) {
        qint32 q = confirmSaveQuestion();
        switch(q) {
        case QMessageBox::Cancel:
            return SaveState::BTN_CANCEL;
        case QMessageBox::No:
            return SaveState::BTN_NO;
        case QMessageBox::Yes:
            SaveState s1, s2, s3;
            s1 = saveChart("chart1", "Chart 1", infoWidget1);
            s2 = saveChart("chart2", "Chart 2", infoWidget2);
            s3 = saveChart("chart3", "Chart 3", infoWidget3);
            if(s1 == SaveState::SAVE_FAIL || s2 == SaveState::SAVE_FAIL || s3 == SaveState::SAVE_FAIL) {
                return SaveState::SAVE_FAIL;
            } else {
                return SaveState::SAVE_SUCCESS;
            }
        default:
            return SaveState::BTN_CANCEL;
        }
    }
    return SaveState::NOT_CHANGED;
}

/**
 * @brief Save all charts slot
 */
void CentralWidget::slotSave() {
    SaveState state = saveAllCharts();
    if(state == SaveState::SAVE_FAIL) {
        qCritical() << "Save Error";
    }
}

/**
 * @brief Display save error
 * @param title - Window title
 * @param err - error type
 */
void CentralWidget::displaySaveError(const QString &title, const SaveErr &err) {
    QString errStr;
    switch(err) {
    case SaveErr::ERR_NOT_WRITABLE:
        errStr = "Configuration file not writable. Check write permissions";
        break;
    case SaveErr::ERR_BEG_TM_COORD:
        errStr = "Begin timemark is not set";
        break;
    case SaveErr::ERR_BEG_TM_TIME:
        errStr = "Time of begin timemark is not set.";
        break;
    case SaveErr::ERR_END_TM_COORD:
        errStr = "End timemark is not set";
        break;
    case SaveErr::ERR_END_TM_TIME:
        errStr = "Time of end timemark is not set.";
        break;
    case SaveErr::ERR_DIRECTION:
        errStr = "Direction not set.";
        break;
    default:
        errStr = "Unknown error";
    }
    QMessageBox::warning(this, title + " error", errStr, QMessageBox::Ok);
}

/**
 * @brief Update chart property if current chart is changed
 * @param p - pointer to ChartProperty
 */
void CentralWidget::updateProperty(ChartProperty &p) {
    QMap<QString, Marker> items = chartWidget->getItems();
    if(items.contains("start_mark")) {
        p.beginTimemarkCoord = items.value("start_mark").coord;
    } else {
        p.beginTimemarkCoord = QPointF();
    }
    if(items.contains("end_mark")) {
        p.endTimemarkCoord = items.value("end_mark").coord;
    } else {
        p.endTimemarkCoord = QPointF();
    }
    if(items.contains("top_border")) {
        p.topBorderCoord = items.value("top_border").coord.y();
    } else {
        p.topBorderCoord = SKIP_VAL;
    }
    if(items.contains("bottom_border")) {
        p.bottomBorderCoord = items.value("bottom_border").coord.y();
    } else {
        p.bottomBorderCoord = SKIP_VAL;
    }
}

/**
 * @brief Widget Changed slot, called from ChartWidget if something is changed
 */
void CentralWidget::slotChartWidgetChanged() {
    ChartProperty p;
    switch(chartSelect->getChart()) {
    case Charts::CHART_ONE:
        p = infoWidget1->getProperty();
        updateProperty(p);
        infoWidget1->setProperty(p);
        infoWidget1->setChanged(true);
        break;
    case Charts::CHART_TWO:
        p = infoWidget2->getProperty();
        updateProperty(p);
        infoWidget2->setProperty(p);
        infoWidget2->setChanged(true);
        break;
    case Charts::CHART_THREE:
        p = infoWidget3->getProperty();
        updateProperty(p);
        infoWidget3->setProperty(p);
        infoWidget3->setChanged(true);
        break;
    }
}

/**
 * @brief Called if info in chart info widget 1 is changed
 */
void CentralWidget::slotInfoWidgetOneChanged() {
    chartWidget->setItems(itemsFromProperty(infoWidget1->getProperty()));
}

/**
 * @brief Called if info in chart info widget 2 is changed
 */
void CentralWidget::slotInfoWidgetTwoChanged() {
    chartWidget->setItems(itemsFromProperty(infoWidget2->getProperty()));
}

/**
 * @brief Called if info in chart info widget s is changed
 */
void CentralWidget::slotInfoWidgetThreeChanged() {
    chartWidget->setItems(itemsFromProperty(infoWidget3->getProperty()));
}

/**
 * @brief Return the current opened file name
 * @return
 */
QString CentralWidget::getCurrentFileName() {
    return currentFile;
}

/**
 * @brief Return true if chart file is opened
 * @return
 */
bool CentralWidget::isOpened() {
    return opened;
}

/**
 * @brief Return true if at least one chart info is unsaved
 * @return
 */
bool CentralWidget::isUnsaved() {
    return infoWidget1->isChanged() || infoWidget2->isChanged() || infoWidget3->isChanged();
}
