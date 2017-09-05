#ifndef CHARTSELECTWIDGET_H
#define CHARTSELECTWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>

enum class Charts {
    CHART_ONE = 0,
    CHART_TWO,
    CHART_THREE
};

class ChartSelectWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartSelectWidget(QWidget *parent = 0);
    Charts getChart();
    void setChartOneName(const QString &name);
    void setChartTwoName(const QString &name);
    void setChartThreeName(const QString &name);

signals:
    void selected();

public slots:

private:
    Charts currentChart;
    QRadioButton *chartOneBtn;
    QRadioButton *chartTwoBtn;
    QRadioButton *chartThreeBtn;

private slots:
    void slotSelectOne();
    void slotSelectTwo();
    void slotSelectThree();
};

#endif // CHARTSELECTWIDGET_H
