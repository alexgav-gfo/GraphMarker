#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QApplication>
#include <QMap>
#include <QWidget>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "chartwidget.h"
#include "chartinfowidget.h"
#include "chartselectwidget.h"
#include "chartpropertyworker.h"
#include "types.h"
#include "settings.h"

enum class SaveState {
    SAVE_SUCCESS,
    SAVE_FAIL,
    BTN_CANCEL,
    BTN_NO,
    NOT_OPENED,
    NOT_CHANGED
};

class CentralWidget : public QWidget {
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);
    QString getCurrentFileName();
    SaveState saveAllCharts();
    bool isOpened();
    bool isUnsaved();

signals:
    void fileOpened();
    void fileClosed();

public slots:
    void slotOpen();
    void slotSave();
    void slotClose();
    void slotNext();
    void slotPrev();
    void slotZoomIn();
    void slotZoomOut();
    void slotZoomOriginal();

private:
    void openFile(const QString &file);
    bool closeFile();
    bool checkFileName(const QString &fileName);
    void updateProperty(ChartProperty &p);
    SaveState saveChart(const QString &section, const QString &title, ChartInfoWidget *info);
    QMap<QString, Marker> itemsFromProperty(const ChartProperty &prop);
    void displaySaveError(const QString &title, const SaveErr &err);
    qint32 confirmSaveQuestion();
    void nextFile(bool direction);

    QString currentFile;
    QStringList currentDir;
    QString propertyFile;
    QHBoxLayout *layout;
    QVBoxLayout *infoLayout;
    ChartWidget *chartWidget;
    ChartInfoWidget *infoWidget1;
    ChartInfoWidget *infoWidget2;
    ChartInfoWidget *infoWidget3;
    ChartSelectWidget *chartSelect;
    bool opened;

private slots:
    void slotChartSelected();
    void slotChartWidgetChanged();
    void slotInfoWidgetOneChanged();
    void slotInfoWidgetTwoChanged();
    void slotInfoWidgetThreeChanged();
};

#endif // CENTRALWIDGET_H
