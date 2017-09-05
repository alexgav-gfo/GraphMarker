#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QDebug>
#include "centralwidget.h"
#include "chartwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initActions();
    void initMainMenu();
    void initToolbar();

    CentralWidget *centralWidget;

    QAction *openAction;
    QAction *closeAction;
    QAction *saveAction;
    QAction *nextChartAction;
    QAction *prevChartAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomOrigAction;
    QAction *exitAction;
    QAction *aboutAction;

    QLabel *fileInfoLabel;

private slots:
    void slotExit();
    void slotAbout();
    void slotFileOpened();
    void slotFileClosed();
};

#endif // MAINWINDOW_H
