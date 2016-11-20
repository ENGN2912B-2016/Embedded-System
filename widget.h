
#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QFile>
#include <QTimer>
#include <QPushButton>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class IOdevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void handleButton();
    void handleStop();

private:
    IOdevice *mic;
    QChart *input_plot;
    QLineSeries *dataseries;
    QAudioInput *audioInput;
    QFile outputfile;
    QPushButton *start_button, *stop_button;
};

#endif // WIDGET_H
