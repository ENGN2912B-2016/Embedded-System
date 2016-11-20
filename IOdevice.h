#ifndef IOdevice_H
#define IOdevice_H

#include <QtCore/QIODevice>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class IOdevice : public QIODevice
{
    Q_OBJECT
public:
    explicit IOdevice(QXYSeries * series, QObject *parent = 0);

protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

private:
    QXYSeries *m_series;
};

#endif // IOdevice_H
