#ifndef RTTCHANNELSETTINGS_H
#define RTTCHANNELSETTINGS_H

#include <QString>

class QCPGraph;
class QFile;

class RTTChannelSettings {
public:
    RTTChannelSettings();
    double      lowerRange;
    double      upperRange;
    qint32      id;
    QString     format;
    QString     color;
    qint32      tableWidgetRow;
    QCPGraph*   graph;
    qint32      msResolution;
    QFile*       file;
};

#endif // RTTCHANNELSETTINGS_H
