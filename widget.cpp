#include "widget.h"
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QCoreApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <qcustomplot/qcustomplot.h>
#include <QDir>
#include <QFile>
#include <QLabel>
#include <iostream>
#include <QSound>
#include <string>
#include <QString>
#include <Qvector>
//#include <soundfile/soundfile.h>



#include "IOdevice.h"

QT_CHARTS_USE_NAMESPACE
using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      mic(0),

      input_plot(0),
      output_plot(0),
      dataseries(0),
      dataseries2(0),
      audioInput(0)
{
    initialize_GUI();
}

Widget::~Widget()
{
    audioInput->stop();
    mic->close();
    speaker->close();
}

void Widget::initialize_GUI()
{
    // Display for input sound waves
    input_plot = new QChart;
    QChartView *InputView = new QChartView(input_plot);
    InputView->setMinimumSize(600, 200);
    dataseries = new QLineSeries;
    input_plot->addSeries(dataseries);
    QValueAxis *axisX_in = new QValueAxis;
    axisX_in->setRange(0, 2000);
    axisX_in->setLabelFormat("%g");
    axisX_in->setTitleText("Samples");
    QValueAxis *axisY_in = new QValueAxis;
    axisY_in->setRange(-1, 1);
    axisY_in->setTitleText("Amplitude");
    input_plot->setAxisX(axisX_in, dataseries);
    input_plot->setAxisY(axisY_in, dataseries);
    input_plot->legend()->hide();
    input_plot->setTitle("Sound waves from microphone");

    // Display for output signals
    //output_plot = new QCustomPlot;
    QChartView *OutputView = new QChartView(output_plot);
    OutputView->setMinimumSize(600, 200);
    dataseries2 = new QLineSeries;
    output_plot->addSeries(dataseries2);
    QValueAxis *axisX_out = new QValueAxis;
    axisX_out->setRange(0, 500);
    axisX_out->setLabelFormat("%g");
    axisX_out->setTitleText("Samples");
    QValueAxis *axisY_out = new QValueAxis;
    axisY_out->setRange(-1, 1);
    axisY_out->setTitleText("Amplitude");
    output_plot->setAxisX(axisX_out, dataseries2);
    output_plot->setAxisY(axisY_out, dataseries2);
    output_plot->legend()->hide();
    output_plot->setTitle("Sound waves after processing");

    // Push Buttons
    // Start button: starts recording audio and displaying input signal
    // Stop button: stopd recording and displaying signals
    // Freq select: toggle button that allows ser to input frequency parameters
    QPushButton *start_button = new QPushButton("Start Recording",this);
    start_button->setStyleSheet("background-color:green;");
    QPushButton *stop_button = new QPushButton("Stop Recording",this);
    stop_button->setStyleSheet("background-color:red;");

    QWidget *widget = new QWidget();
    QPushButton *Freq_select = new QPushButton(this);
    //Freq_select->setCheckable(true);
    Freq_select->setMinimumWidth(200);
    Freq_select->setText("Play");
    Text1=new QTextEdit();
    Text1->setMinimumHeight(25);
    Text1->setText("0");
    QGridLayout * layout = new QGridLayout(widget);
    QFont f("Default",10);
    QLabel *label1= new QLabel("Frequency scale:");
    label1->setFont(f);
    //label1->setBold(true);
    layout->addWidget(label1,0,0);
    layout->addWidget(Text1,0,1);

    QWidget *widget1 = new QWidget();
    QGridLayout * layout1 = new QGridLayout(widget1);
    layout1->addWidget(start_button,0,0);
    layout1->addWidget(stop_button,0,1);
    layout1->addWidget(Freq_select,0,2);

    Text2=new QTextEdit();
    Text2->setMinimumHeight(25);

    Text2->setText("Audio Program started.");
    // Main GUI box
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(InputView);
    mainLayout->addWidget(OutputView);
    mainLayout->addWidget(widget);
    mainLayout->addWidget(widget1);
    //mainLayout->addWidget(start_button);
    //mainLayout->addWidget(stop_button);
    mainLayout->addWidget(new QLabel("Message Box:"));
    mainLayout->addWidget(Text2);
    mainLayout->addWidget(customPlot);
    setLayout(mainLayout);

    // Connect button signal to appropriate slot
    connect(start_button, SIGNAL (released()), this, SLOT (handleStart()));
    connect(stop_button, SIGNAL (released()), this, SLOT (handleStop()));
    connect(Freq_select, SIGNAL (released()), this, SLOT (FreqSelect()));


}


void Widget::handleStart()
{
    Write_scale();
    qDebug()<<"Recording from...";
    Text2->setText("Recording from mic...");
    outputfile.setFileName("/audio_qt.raw");
    outputfile.open( QIODevice::WriteOnly | QIODevice::Truncate );
    QAudioFormat formatAudio;
    formatAudio.setSampleRate(44100);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
    audioInput = new QAudioInput(inputDevices,formatAudio, this);
    audioRecord=new QAudioRecorder(this);
    qDebug()<<inputDevices.deviceName();

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setSampleRate(44100);
    mic = new IOdevice(dataseries, this);
    mic->open(QIODevice::WriteOnly);

    start= clock();

    QString path = QDir::currentPath();
    path.append("/input");

    audioInput->start(mic);
    audioRecord->setEncodingSettings(audioSettings);
    audioRecord->setOutputLocation(QUrl::fromLocalFile(path));
    audioRecord->record();
}

void Widget::handleStop()
{
    finish=clock();
    int time=finish-start;
    if (time<=5000)
    {
       qWarning()<<"Recording less than 5 seconds.";
       Text2->setText("Recording has to be atleast 5 seconds.");
       audioInput->stop();
       audioRecord->stop();
       outputfile.close();
       delete audioInput;
       delete audioRecord;
    }
    else
    {
        Text2->setText("Recording stopped successfully.");
        qDebug()<<"Stopping mic...";
        audioInput->stop();
        audioRecord->stop();
        outputfile.close();
        delete audioInput;
        delete audioRecord;
        //Write_File();
    }
    cout<<dataseries<<endl;
}

void Widget::FreqSelect()
{
    //updateText();
//    cout<<"WTF?? "<< endofFile<<endl;
    samplecount=0;
    //while (samplecount<2000){
//        cout<<"in"<<endl;
        //Read_File();
//        cout<<"endoffile"<<endofFile<<endl;
//  }
   // }
    Play();



}

void Widget::Read_File()
{
    //demoName = "Quadratic Demo";
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
}

//    Text2->setText("Reading file...");
//    //  QVector<double>data;
//    //QVector<double>sample;
//    //double data=0;
//    double sample=0;
//    QFile file("Data1.txt");
//    file.open(QFile::ReadOnly | QFile::Text);
//     QTextStream ReadFile(&file);
//    // ReadFile.seek(samplecount);
//     while(sample<2000)
//     {
//            QString line = ReadFile.readLine();
//            data.push_back(line.toDouble());
//            //dataseries2->append(sample,data);
//            sample=sample+1;
//            samples.push_back(sample);
//            samplecount=samplecount+1;
//            //datalist<<QPointF(samplecount,data);
//     }
//     customPlot->addGraph();
//     customPlot->graph(0)->setData(samples, data);
//     // give the axes some labels:
//     customPlot->xAxis->setLabel("x");
//     customPlot->yAxis->setLabel("y");
//     // set axes ranges, so we see all data:
//     customPlot->xAxis->setRange(0, 2000);
//     customPlot->yAxis->setRange(-1,1);
//   }



//    start= clock();
//    cout<<"WTF Part 2.."<<endl;
//    Text2->setText("Reading file...");
//    //  QVector<double>data;
//    QVector<double>sample;
//    double data=0;
//    double sample=0;
//    QFile file("Data1.txt");
//    file.open(QFile::ReadOnly | QFile::Text);
//     QTextStream ReadFile(&file);
//     ReadFile.seek(samplecount);
//     while(sample<2000){
//            QString line = ReadFile.readLine();
//            data=line.toDouble();
//            //dataseries2->append(sample,data);
//            sample=sample+1;
//            samplecount=samplecount+1;
//            datalist<<QPointF(samplecount,data);
//        }
//       file.close();
//       cout<<"Sample count"<<samplecount<<endl;
//       Text2->setText("Plotting Graph... ");
//       QValueAxis *axisX_out = new QValueAxis;
//       axisX_out->setRange(0, sample);
//       axisX_out->setLabelFormat("%g");
//       axisX_out->setTitleText("Samples");
//       QValueAxis *axisY_out = new QValueAxis;
//       axisY_out->setRange(-1, 2);
//       axisY_out->setTitleText("Amplitude");
//       dataseries2->append(datalist);
//       output_plot->setAxisX(axisX_out, dataseries2);
//       output_plot->setAxisY(axisY_out, dataseries2);

//       finish=clock();
//       int time=(finish-start)/1000;
//       cout<<"elapsed time "<<time<<" seconds."<<endl;



void Widget::Write_scale()
{
    //ofstream file("scale.txt"); //put your program together with thsi file in the same folder.
    QFile file("scale.txt");
    //file.open(QFile::ReadWrite);
    if(file.open(QFile::ReadWrite))
    {
        QString input = Text1->toPlainText();
        double scale=input.toDouble();
        QTextStream out(&file);
        out<<scale<<endl;
    }
    else
    {
        qWarning()<<"Could not save frequency scale. Output file does not exist";
    }
}

//void Widget::Write_File()
//{
//    //QFile text=new QFile
//    QFile File("input.wav");
//    File.open(QIODevice::ReadWrite);


//    char* strm;
//    QVector<double> m_DATA;
//    while(!File.atEnd())
//    {
//    File.read(strm,4);
//    //if(qFromLittleEndian<short>((uchar*)strm))
//    m_DATA << strm;
//    //}
//    cout<<m_DATA<<endl;
//}
//}

void Widget::Play()
{
    Text2->setText("Playing sound file.");
    QFile audio_file("input.wav");

        if(audio_file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"Playing sound from...";
            QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
            qDebug() << info.deviceName();
            QSound::play("input.wav");
        }

        else
        {
            qWarning()<<"Could not open sound file.Invalid file/format.";
        }

}

void Widget::updateText()
{
    this->Freq_select->setText("Pause");
}
