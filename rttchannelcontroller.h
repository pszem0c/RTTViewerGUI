#ifndef RTTCHANNELCONTROLLER_H
#define RTTCHANNELCONTROLLER_H


#include <QList>

class RTTChannelSettings;
class QTableWidget;
class QCustomPlot;

class RTTChannelController {
private:
    QList<RTTChannelSettings*> channelSettingsList;
    QTableWidget* table;
    QCustomPlot* plot;
    qint32 idCount;
    qint32 plotTimeHorizon;


    void addChannelToTable(RTTChannelSettings* settings);
    void addChannelToPlot(RTTChannelSettings* settings);

public:
    RTTChannelController(QTableWidget* tableWidget, QCustomPlot* customPlot);

    void addChannel(RTTChannelSettings* settings);
    void updateChannelValue(qint32 id, qint32 value);
    void updateChannels(QList<QPair<qint32, qint32> > dataList);
    void tableValueChanged(int row, int column);
};

#endif // RTTCHANNELCONTROLLER_H
