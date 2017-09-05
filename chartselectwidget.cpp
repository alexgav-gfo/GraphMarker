#include "chartselectwidget.h"

/**
 * @brief Chart Selector - Constructor
 * @param parent
 */
ChartSelectWidget::ChartSelectWidget(QWidget *parent) : QWidget(parent) {
    currentChart = Charts::CHART_TWO;
    QFont f("sans-serif", 10, QFont::Normal);
    QGroupBox *box = new QGroupBox(tr("Select Chart"), this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QButtonGroup *group = new QButtonGroup(this);
    chartOneBtn = new QRadioButton("Chart 1", this);
    chartOneBtn->setFont(f);
    connect(chartOneBtn, &QRadioButton::clicked, this, &ChartSelectWidget::slotSelectOne);
    group->addButton(chartOneBtn);
    layout->addWidget(chartOneBtn);
    chartTwoBtn = new QRadioButton("Chart 2", this);
    chartTwoBtn->setFont(f);
    chartTwoBtn->setChecked(true);
    connect(chartTwoBtn, &QRadioButton::clicked, this, &ChartSelectWidget::slotSelectTwo);
    group->addButton(chartTwoBtn);
    layout->addWidget(chartTwoBtn);
    chartThreeBtn = new QRadioButton("Chart 3", this);
    chartThreeBtn->setFont(f);
    connect(chartThreeBtn, &QRadioButton::clicked, this, &ChartSelectWidget::slotSelectThree);
    group->addButton(chartThreeBtn);
    layout->addWidget(chartThreeBtn);
    box->setLayout(layout);
    QGridLayout *grid = new QGridLayout(this);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->addWidget(box);
    setLayout(grid);
}

/**
 * @brief Get selected chart
 * @return
 */
Charts ChartSelectWidget::getChart() {
    return currentChart;
}

/**
 * @brief Set name for chart 1
 * @param name
 */
void ChartSelectWidget::setChartOneName(const QString &name) {
    chartOneBtn->setText("# " + name.toUpper());
}

/**
 * @brief Set name for chart 2
 * @param name
 */
void ChartSelectWidget::setChartTwoName(const QString &name) {
    chartTwoBtn->setText("# " + name.toUpper());
}

/**
 * @brief Set name for chart 3
 * @param name
 */
void ChartSelectWidget::setChartThreeName(const QString &name) {
    chartThreeBtn->setText("# " + name.toUpper());
}

/**
 * @brief Chart 1 selected slot
 */
void ChartSelectWidget::slotSelectOne() {
    currentChart = Charts::CHART_ONE;
    emit selected();
}

/**
 * @brief Chart 2 selected slot
 */
void ChartSelectWidget::slotSelectTwo() {
    currentChart = Charts::CHART_TWO;
    emit selected();
}

/**
 * @brief Chart 3 selected slot
 */
void ChartSelectWidget::slotSelectThree() {
    currentChart = Charts::CHART_THREE;
    emit selected();
}
