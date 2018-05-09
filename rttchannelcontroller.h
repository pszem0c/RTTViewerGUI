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

    void addChannelToTable(RTTChannelSettings* settings);

public:
    RTTChannelController(QTableWidget* tableWidget, QCustomPlot* customPlot);

    void addChannel(RTTChannelSettings* settings);
    void updateChannelValue(qint32 id, qint32 value);
    void tableValueChanged(int row, int column);
};

#endif // RTTCHANNELCONTROLLER_H
