#include "rttchannellogger.h"
#include "rttchannelsettings.h"
#include <QFile>
#include <QTime>
#include <QDateTime>

RTTChannelLogger::RTTChannelLogger() {

}

RTTChannelLogger::~RTTChannelLogger() {
     for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        (*it)->file->close();
     }
}


void RTTChannelLogger::logData(QList<QPair<qint32, float> > dataList) {
    QTime time ;
    qint32 prevChannelId = -1;
    for (QList<QPair<qint32, float> >::iterator itData = dataList.begin(); itData != dataList.end(); itData++) {
        for (QList<RTTChannelSettings*>::iterator itChannel = channelSettingsList.begin(); itChannel != channelSettingsList.end(); itChannel++) {
            time = QTime::currentTime();
            if ((*itChannel)->id == (*itData).first) {
                if (prevChannelId == (*itChannel)->id) {
                    time.addMSecs((*itChannel)->msResolution);
                }
                (*itChannel)->file->write(QString(QString::number(time.msecsSinceStartOfDay()) + "," + QString::number(itData->second)+"\r\n").toLocal8Bit()) ;
                break;
            }
        }
    }
}

void RTTChannelLogger::addChannel(RTTChannelSettings *settings) {
    channelSettingsList.append(settings);
    settings->file = new QFile(settings->format + "-" + QDateTime::currentDateTime().toString("ddMMyy-hhmm") +".log");
    settings->file->open(QIODevice::WriteOnly | QIODevice::Truncate);
}
