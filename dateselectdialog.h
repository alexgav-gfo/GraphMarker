#ifndef DATESELECTDIALOG_H
#define DATESELECTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCalendarWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateTime>
#include <QDebug>

class DateSelectDialog : public QDialog {
    Q_OBJECT
public:
    explicit DateSelectDialog(QDialog *parent = 0);
    ~DateSelectDialog();
    QDateTime getTime();
    void setTime(const QDateTime &time);

signals:

public slots:

private:
    void makeTimeLabel();

    QDateTime dateTime;
    QLabel *title;
    QCalendarWidget *calendar;
    QListWidget *hours;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

private slots:
    void slotCalendarSelected();
    void slotTimeSelected();
    void slotOk();
    void slotCancel();
};

#endif // DATESELECTDIALOG_H
