#ifndef RTTCHANNELSETTINGS_H
#define RTTCHANNELSETTINGS_H

#include <QString>

class RTTChannelSettings {
public:
    RTTChannelSettings();
    double lowerRange;
    double upperRange;
    qint32 id;
    QString format;
    QString color;
};

#endif // RTTCHANNELSETTINGS_H
