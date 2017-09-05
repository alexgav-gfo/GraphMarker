#include "dateselectdialog.h"

/**
 * @brief DateSelectDialog constructor
 * @param parent
 */
DateSelectDialog::DateSelectDialog(QDialog *parent) : QDialog(parent) {
    calendar = new QCalendarWidget;
    connect(calendar, &QCalendarWidget::clicked, this, &DateSelectDialog::slotCalendarSelected);
    QHBoxLayout *calendarLayout = new QHBoxLayout;

    calendarLayout->addWidget(calendar);
    hours = new QListWidget;
    connect(hours, &QListWidget::clicked, this, &DateSelectDialog::slotTimeSelected);
    hours->setFixedWidth(70);
    for(qint32 i = 0; i < 24; ++i) {
        hours->insertItem(i, QString().sprintf("%02d:00", i));
    }
    hours->setCurrentRow(0);
    calendarLayout->addWidget(hours);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    okBtn = new QPushButton(tr("&Ok"));
    connect(okBtn, &QPushButton::clicked, this, &DateSelectDialog::slotOk);
    btnLayout->addWidget(okBtn);
    cancelBtn = new QPushButton(tr("&Cancel"));
    connect(cancelBtn, &QPushButton::clicked, this, &DateSelectDialog::slotCancel);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout *layout = new QVBoxLayout;
    title = new QLabel(tr("Select Time"));
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont("sans-serif", 18, QFont::Normal));
    layout->addWidget(title);
    layout->addLayout(calendarLayout);
    layout->addLayout(btnLayout);
    setLayout(layout);
    adjustSize();
}

/**
 * @brief DateSelectDialog destructor
 */
DateSelectDialog::~DateSelectDialog() {
    delete calendar;
    delete hours;
    delete cancelBtn;
    delete okBtn;
}

/**
 * @brief Get selected time
 * @return
 */
QDateTime DateSelectDialog::getTime() {
    return dateTime;
}

/**
 * @brief Set time (calculated from filename)
 * @param time
 */
void DateSelectDialog::setTime(const QDateTime &time) {
    calendar->setSelectedDate(time.date());
    qint32 hour = time.time().hour();
    hours->setCurrentRow(hour);
    makeTimeLabel();
}

/**
 * @brief Make text for time label
 */
void DateSelectDialog::makeTimeLabel() {
    QString date = calendar->selectedDate().toString("yyyy-MM-dd");
    QString time = hours->currentItem()->text();
    title->setText("Date:  " + date + "  " + time);
}

/**
 * @brief Calendar widget info selected
 */
void DateSelectDialog::slotCalendarSelected() {
    makeTimeLabel();
}

/**
 * @brief Time widget info selected
 */
void DateSelectDialog::slotTimeSelected() {
    makeTimeLabel();
}

/**
 * @brief Ok button pressed
 */
void DateSelectDialog::slotOk() {
    QDate date = calendar->selectedDate();
    QTime time = QTime::fromString(hours->currentItem()->text(), "HH:mm");
    dateTime = QDateTime(date, time);
    this->accept();
}

/**
 * @brief Cancel button pressed
 */
void DateSelectDialog::slotCancel() {
    this->reject();
}
