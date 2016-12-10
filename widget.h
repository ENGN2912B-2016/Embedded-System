
#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QValueAxis>
#include <QFile>
#include <QDebug>
#include <QAudioOutput>
#include <QTimer>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QTextStream>
#include <QAudioRecorder>
#include <qcustomplot/qcustomplot.h>
#include <QMediaPlayer>
#include <QElapsedTimer>
#include <QAudioBuffer>
#include <fstream>
#include <QVector>
#include <string>
#include <ctime>

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
    void initialize_GUI();
    void Play();
    void Read_File();
    void Write_scale();
    void Write_File();
    void updateText();
    int samplecount=0;
    int endofFile=0; //1 is end of file
    QVector<double> data;
    QVector<double> samples;

private slots:
    void handleStart();
    void handleStop();
    void FreqSelect();

private:
    IOdevice *mic, *speaker;
    QChart *input_plot;
    QChart *output_plot;
    QLineSeries *dataseries;
    QByteArray *audio_data;
    QLineSeries *dataseries2;
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    QFile outputfile;
    QPushButton *start_button, *stop_button, *Freq_select;
    QTextEdit *Text1,*Text2;
    QWidget *widget;
    QToolButton *b;
    QAudioBuffer *buffer;
    QTextStream *stream;
    QAudioRecorder *audioRecord;
    QMediaPlayer *player;
    clock_t start,finish;
    QCustomPlot *customPlot;

};

#endif // WIDGET_H

