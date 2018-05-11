#ifndef RTTCHANNELLOGGER_H
#define RTTCHANNELLOGGER_H


#include <QList>

class RTTChannelSettings;

class RTTChannelLogger {
private:
    QList<RTTChannelSettings*> channelSettingsList;
public:
    RTTChannelLogger();
    ~RTTChannelLogger();

    void logData(QList<QPair<qint32, qint32> > dataList);
    void addChannel(RTTChannelSettings *settings);
};

#endif // RTTCHANNELLOGGER_H
