#include "IOdevice.h"
#include <QtCharts/QXYSeries>
#include <QTextStream>
#include <iostream>
#include <QFile>

using namespace std;
IOdevice::IOdevice(QXYSeries * series, QObject *parent) :
    QIODevice(parent),
    m_series(series)
{

}

qint64 IOdevice::readData(char * data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 IOdevice::writeData(const char * data, qint64 maxSize)
{
    qint64 range = 2000;
    QVector<QPointF> oldPoints = m_series->pointsVector();
    QVector<QPointF> points;
    int resolution = 4;

    if (oldPoints.count() < range) {
        points = m_series->pointsVector();
    } else {
        for (int i = maxSize/resolution; i < oldPoints.count(); i++)
            points.append(QPointF(i - maxSize/resolution, oldPoints.at(i).y()));
    }

    qint64 size = points.count();
    for (int k = 0; k < maxSize/resolution; k++)
        points.append(QPointF(k + size, ((quint8)data[resolution * k] - 128)/128.0));

    m_series->replace(points);
    QString filename = "Data.txt";
    QFile file(filename);

    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<m_series<<endl;

    }
   // cout<<m_series<<endl;
    return maxSize;
}
