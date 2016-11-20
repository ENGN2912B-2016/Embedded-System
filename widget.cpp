#include "widget.h"
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QFile>
#include <QPushButton>
#include <QTimer>
#include <iostream>
#include "IOdevice.h"

QT_CHARTS_USE_NAMESPACE
using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      mic(0),
      input_plot(0),
      dataseries(0),
      audioInput(0)
{
    input_plot = new QChart;
    QChartView *chartView = new QChartView(input_plot);
    chartView->setMinimumSize(600, 400);
    dataseries = new QLineSeries;
    input_plot->addSeries(dataseries);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 2000);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Amplitude");
    input_plot->setAxisX(axisX, dataseries);
    input_plot->setAxisY(axisY, dataseries);
    input_plot->legend()->hide();
    input_plot->setTitle("Sound waves from microphone");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);

    QPushButton *start_button = new QPushButton("Start Recording",this);
    start_button->setGeometry(QRect(QPoint(50, 50),QSize(100, 25)));
    mainLayout->addWidget(start_button);

    QPushButton *stop_button = new QPushButton("Stop Recording");
    mainLayout->addWidget(stop_button);

    // Connect button signal to appropriate slot
    connect(start_button, SIGNAL (released()), this, SLOT (handleButton()));
    connect(stop_button, SIGNAL (released()), this, SLOT (handleStop()));

}

Widget::~Widget()
{
    audioInput->stop();
    mic->close();
}

void Widget::handleButton()
{
    cout<<"Recording..."<<endl;
    outputfile.setFileName("/audio_qt.raw");
    outputfile.open( QIODevice::WriteOnly | QIODevice::Truncate );
    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
    audioInput = new QAudioInput(inputDevices,formatAudio, this);

    mic = new IOdevice(dataseries, this);
    mic->open(QIODevice::WriteOnly);

    audioInput->start(mic);

}

void Widget::handleStop()
{
    cout<<"Stopping..."<<endl;
    audioInput->stop();
    outputfile.close();
    delete audioInput;
}
