#include "rttchannelcontroller.h"
#include <QTableWidget>
#include "qcustomplot.h"
#include "rttchannelsettings.h"
#include <QObject>
#include <QTime>
#include <QFile>
#include <QTextStream>

RTTChannelController::RTTChannelController(QTableWidget *tableWidget, QCustomPlot *customPlot) {
    table = tableWidget;
    plot = customPlot;
    idCount = 0;
    plotTimeHorizon = 10000;

    table->setColumnCount(6);
    table->setRowCount(0);
    table->setHorizontalHeaderLabels(QStringList() << "ID" << "Format" << "Value" << "Lower range" << "Upper range" << "Color");

    plot->axisRect()->removeAxis(plot->yAxis);
    qint32 time = QTime::currentTime().msecsSinceStartOfDay();
    plot->xAxis->setRange(time-plotTimeHorizon, time);
}


void RTTChannelController::addChannelToTable(RTTChannelSettings *settings) {
    table->blockSignals(true);
    qint32 row = table->rowCount();
    settings->tableWidgetRow = row;
    table->setRowCount(row+1);
    QTableWidgetItem* idItem = new QTableWidgetItem(QObject::tr("%1").arg(settings->id));
    idItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* formatItem = new QTableWidgetItem(QObject::tr("%1").arg(settings->format));
    formatItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* valueItem = new QTableWidgetItem(QObject::tr("%1").arg(0));
    valueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* lowerRangeItem = new QTableWidgetItem(QObject::tr("%1").arg(settings->lowerRange));
    QTableWidgetItem* upperRangeItem = new QTableWidgetItem(QObject::tr("%1").arg(settings->upperRange));
    QTableWidgetItem* colorItem = new QTableWidgetItem(QObject::tr("%1").arg(settings->color));
    table->setItem(row, 0, idItem);
    table->setItem(row, 1, formatItem);
    table->setItem(row, 2, valueItem);
    table->setItem(row, 3, lowerRangeItem);
    table->setItem(row, 4, upperRangeItem);
    table->setItem(row, 5, colorItem);
    table->blockSignals(false);
}

void RTTChannelController::addChannelToPlot(RTTChannelSettings *settings) {
    QCPAxis* valAxis = plot->axisRect()->addAxis(QCPAxis::atLeft);
    valAxis->setBasePen(QPen(QColor(settings->color)));
    valAxis->setLabel(settings->format);
    valAxis->setTickPen(QPen(QColor(settings->color)));
    valAxis->setSubTickPen(QPen(QColor(settings->color)));
    valAxis->setPadding(25);
    valAxis->setRange(settings->lowerRange, settings->upperRange);

    settings->graph = plot->addGraph(plot->xAxis, valAxis);
    settings->graph->setPen(QPen(QColor(settings->color)));

    plot->replot();
}

void RTTChannelController::addChannel(RTTChannelSettings *settings) {
    channelSettingsList.append(settings);
    settings->id = idCount++;
    addChannelToTable(settings);
    addChannelToPlot(settings);
}

void RTTChannelController::updateChannelValue(qint32 id, qint32 value) {
    qint32 time = QTime::currentTime().msecsSinceStartOfDay();
    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        if ((*it)->id == id) {
            table->item((*it)->tableWidgetRow,2)->setText(QString::number(value));
            plot->xAxis->setRange(time-plotTimeHorizon, time);
            (*it)->graph->addData(time, value);
            plot->replot();
            break;
        }
    }
}

void RTTChannelController::updateChannels(QList<QPair<qint32, float> > dataList) {
    qint32 time ;
    qint32 prevChannelId = -1;
    for (QList<QPair<qint32, float> >::iterator itData = dataList.begin(); itData != dataList.end(); itData++) {
        for (QList<RTTChannelSettings*>::iterator itChannel = channelSettingsList.begin(); itChannel != channelSettingsList.end(); itChannel++) {
            time = QTime::currentTime().msecsSinceStartOfDay();
            if ((*itChannel)->id == (*itData).first) {
                if (prevChannelId == (*itChannel)->id) {
                    time += (*itChannel)->msResolution;
                }
                prevChannelId = (*itChannel)->id;
                table->item((*itChannel)->tableWidgetRow,2)->setText(QString::number((*itData).second));
                (*itChannel)->graph->addData(time, (*itData).second);
                break;
            }
        }
    }

    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        (*it)->graph->data()->removeBefore(time-plotTimeHorizon);
    }

    plot->xAxis->setRange(time-plotTimeHorizon, time);
    plot->replot();
}

void RTTChannelController::tableValueChanged(int row, int column) {
    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        if ((*it)->tableWidgetRow == row) {
            if (column == 3) {
                (*it)->lowerRange = table->item(row,column)->text().toDouble();
                (*it)->graph->valueAxis()->setRangeLower((*it)->lowerRange);
                plot->replot();
            } else if (column == 4) {
                (*it)->upperRange = table->item(row,column)->text().toDouble();
                (*it)->graph->valueAxis()->setRangeUpper((*it)->upperRange);
                plot->replot();
            } else if (column == 5) {

            }
            break;
        }
    }
}

void RTTChannelController::saveConfigurationToFile(QString fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&file);
        for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
            stream << (*it)->lowerRange << ","
                   << (*it)->upperRange << ","
                   << (*it)->format << ","
                   << (*it)->color << ","
                   << (*it)->msResolution << endl;
        }
    }
}

QList<RTTChannelSettings *> RTTChannelController::loadConfigurationFromFile(QString fileName) {
    QFile file(fileName);
    if (file.open((QIODevice::ReadOnly))) {
        QTextStream stream(&file);
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList args = line.split(",");
            RTTChannelSettings* settings = new RTTChannelSettings;
            settings->lowerRange    = ((QString)args.at(0)).toDouble();
            settings->upperRange    = ((QString)args.at(1)).toDouble();
            settings->format        = (QString)args.at(2);
            settings->color         = (QString)args.at(3);
            settings->msResolution  = ((QString)args.at(4)).toInt();
            addChannel(settings);
        }
    }
   return channelSettingsList;
}


