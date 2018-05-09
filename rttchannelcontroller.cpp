#include "rttchannelcontroller.h"
#include <QTableWidget>
#include "qcustomplot.h"
#include "rttchannelsettings.h"


RTTChannelController::RTTChannelController(QTableWidget *tableWidget, QCustomPlot *customPlot) {
    table = tableWidget;
    plot = customPlot;
    idCount = 0;

    table->setColumnCount(6);
    table->setRowCount(0);
    table->setHorizontalHeaderLabels(QStringList() << "ID" << "Format" << "Value" << "Lower range" << "Upper range" << "Color");
}


void RTTChannelController::addChannelToTable(RTTChannelSettings *settings) {
    table->blockSignals(true);
    qint32 row = table->rowCount();
    settings->tableWidgetRow = row+1;
    table->setRowCount(row+1);
    QTableWidgetItem* idItem = new QTableWidgetItem(tr("%1").arg(settings->id));
    idItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* formatItem = new QTableWidgetItem(tr("%1").arg(settings->format));
    formatItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* valueItem = new QTableWidgetItem(tr("%1").arg(0));
    valueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* lowerRangeItem = new QTableWidgetItem(tr("%1").arg(settings->lowerRange));
    QTableWidgetItem* upperRangeItem = new QTableWidgetItem(tr("%1").arg(settings->upperRange));
    QTableWidgetItem* colorItem = new QTableWidgetItem(tr("%1").arg(settings->color));
    table->setItem(row, 0, idItem);
    table->setItem(row, 1, formatItem);
    table->setItem(row, 2, valueItem);
    table->setItem(row, 3, lowerRangeItem);
    table->setItem(row, 4, upperRangeItem);
    table->setItem(row, 5, colorItem);
    table->blockSignals(false);
}

void RTTChannelController::addChannel(RTTChannelSettings *settings) {
    channelSettingsList.append(settings);
    settings->id = idCount++;
    addChannelToTable(settings);
}

void RTTChannelController::updateChannelValue(qint32 id, qint32 value) {
    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        if ((*it)->id == id) {
            table->item((*it)->tableWidgetRow,2)->setText(QString::number(value));
        }
    }
}

void RTTChannelController::tableValueChanged(int row, int column) {
    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
        if ((*it)->tableWidgetRow == row) {
            if (column == 3) {
                (*it)->lowerRange = table->item(row,column)->text().toDouble();
            } else if (column == 4) {
                (*it)->upperRange = table->item(row,column)->text().toDouble();
            } else if (column == 5) {

            }
        }
    }
}


