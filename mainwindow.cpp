#include "mainwindow.h"

/**
 * @brief MainWindow constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new CentralWidget();
    connect(centralWidget, &CentralWidget::fileOpened, this, &MainWindow::slotFileOpened);
    connect(centralWidget, &CentralWidget::fileClosed, this, &MainWindow::slotFileClosed);
    this->setCentralWidget(centralWidget);
    initActions();
    initMainMenu();
    initToolbar();
}

/**
 * @brief MainWindow destructor
 */
MainWindow::~MainWindow() {
}

/**
 * @brief Close event for application
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    if(centralWidget->isOpened() && centralWidget->isUnsaved()) {
        QMessageBox box(this);
        box.setText(tr("Confirm Close"));
        box.setInformativeText(tr("Some changes is unsaved.\n Do you really want to close application ?"));
        box.setIcon(QMessageBox::Question);
        box.setStandardButtons(QMessageBox::Close | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Cancel);
        qint32 ret = box.exec();
        ret == QMessageBox::Close ? event->accept() : event->ignore();
    } else {
        event->accept();
    }
}

/**
 * @brief Init actions
 */
void MainWindow::initActions() {
    openAction = new QAction(QIcon(":icons/document-open.png"), tr("&Open"));
    connect(openAction, &QAction::triggered, centralWidget, &CentralWidget::slotOpen);
    closeAction = new QAction(QIcon(":icons/document-close.png"), tr("&Close"));
    connect(closeAction, &QAction::triggered, centralWidget, &CentralWidget::slotClose);
    saveAction = new QAction(QIcon(":icons/document-save.png"), tr("&Save"));
    connect(saveAction, &QAction::triggered, centralWidget, &CentralWidget::slotSave);
    nextChartAction = new QAction(QIcon(":icons/go-next.png"), tr("&Next Chart"));
    connect(nextChartAction, &QAction::triggered, centralWidget, &CentralWidget::slotNext);
    prevChartAction = new QAction(QIcon(":icons/go-previous.png"), tr("&Previous Chart"));
    connect(prevChartAction, &QAction::triggered, centralWidget, &CentralWidget::slotPrev);
    zoomInAction = new QAction(QIcon(":icons/zoom-in.png"), tr("Zoom &In"));
    connect(zoomInAction, &QAction::triggered, centralWidget, &CentralWidget::slotZoomIn);
    zoomOutAction = new QAction(QIcon(":icons/zoom-out.png"), tr("Zoom &Out"));
    connect(zoomOutAction, &QAction::triggered, centralWidget, &CentralWidget::slotZoomOut);
    zoomOrigAction = new QAction(QIcon(":icons/zoom-original.png"), tr("&Zoom Original"));
    connect(zoomOrigAction, &QAction::triggered, centralWidget, &CentralWidget::slotZoomOriginal);
    exitAction = new QAction(QIcon(":icons/application-exit.png"), tr("&Exit"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::slotExit);
    aboutAction = new QAction(QIcon(":icons/help-about.png"), tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::slotAbout);
}

/**
 * @brief Init main menu
 */
void MainWindow::initMainMenu() {
    QMenuBar *mm = this->menuBar();
    QMenu *fileMenu = mm->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(prevChartAction);
    fileMenu->addAction(nextChartAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    QMenu *chartMenu = mm->addMenu(tr("&Chart"));
    chartMenu->addAction(zoomInAction);
    chartMenu->addAction(zoomOutAction);
    chartMenu->addAction(zoomOrigAction);
    QMenu *helpMenu = mm->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

/**
 * @brief Init toolbar
 */
void MainWindow::initToolbar() {
    QToolBar *toolBar = this->addToolBar(tr("Main"));
    toolBar->setIconSize(QSize(32, 32));
    toolBar->addAction(openAction);
    toolBar->addAction(closeAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(prevChartAction);
    toolBar->addAction(nextChartAction);
    toolBar->addSeparator();
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
    toolBar->addAction(zoomOrigAction);
    toolBar->addSeparator();
    fileInfoLabel = new QLabel(tr("Press 'Open' button to open file"));
    fileInfoLabel->setContentsMargins(50, 0, 0, 0);
    fileInfoLabel->setFont(QFont("sans-serif", 24));
    toolBar->addWidget(fileInfoLabel);
}

/**
 * @brief File opened slot (from CentralWidget), display file name
 */
void MainWindow::slotFileOpened() {
    QString file = centralWidget->getCurrentFileName();
    fileInfoLabel->setText("File: " + file);
}

/**
 * @brief File closed slot (from CentralWidget)
 */
void MainWindow::slotFileClosed() {
    fileInfoLabel->setText(tr("File closed"));
}

/**
 * @brief Exit application
 */
void MainWindow::slotExit() {
    close();
}

/**
 * @brief About application
 */
void MainWindow::slotAbout() {
    QMessageBox::information(this, "Chart Marker", "Utility for marking chart recorder scans.\n"
                                                   "\u00A9 GML 2017\n"
                                                   "Powered by Qt ver. " + QString(QT_VERSION_STR));
}
