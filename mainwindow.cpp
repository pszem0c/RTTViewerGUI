#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../RTTViewer/rttconnector.h"
#include "../RTTViewer/rttparser.h"
#include "addchanneldialog.h"
#include "rttchannelsettings.h"

#include <QStringList>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    rttConnector = new RTTConnector();
    rttParser = new RTTParser();
    idCount = 0;
    plotTimeHorizon = 10000;

    connect(rttConnector, SIGNAL(lineRead(QString)), rttParser, SLOT(parseSlot(QString)));
    connect(rttParser, SIGNAL(lineParsed(qint32,qint32)),this, SLOT(updateData(qint32,qint32)));

    rttConnector->connectToHost();
    ui->channelTable->setColumnCount(6);
    ui->channelTable->setRowCount(0);
    ui->channelTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Format" << "Value" << "Lower range" << "Upper range" << "Color");
}

MainWindow::~MainWindow() {
    rttConnector->disconnectFromHost();
    delete rttConnector;
    delete rttParser;
    delete ui;
}


void MainWindow::on_addChannelButton_clicked() {
    AddChannelDialog* addChannelDialog = new AddChannelDialog(this);
    connect(addChannelDialog, SIGNAL(addChannelSettings(RTTChannelSettings*)), this, SLOT(addChannelSettings(RTTChannelSettings*)));
    addChannelDialog->exec();
}

void MainWindow::addChannelToTableView(RTTChannelSettings *settings){
    ui->channelTable->blockSignals(true);
    qint32 row = ui->channelTable->rowCount();
    ui->channelTable->setRowCount(row+1);
    QTableWidgetItem* idItem = new QTableWidgetItem(tr("%1").arg(settings->id));
    idItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* formatItem = new QTableWidgetItem(tr("%1").arg(settings->format));
    formatItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* valueItem = new QTableWidgetItem(tr("%1").arg(0));
    valueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QTableWidgetItem* lowerRangeItem = new QTableWidgetItem(tr("%1").arg(settings->lowerRange));
    QTableWidgetItem* upperRangeItem = new QTableWidgetItem(tr("%1").arg(settings->upperRange));
    QTableWidgetItem* colorItem = new QTableWidgetItem(tr("%1").arg(settings->color));
    ui->channelTable->setItem(row, 0, idItem);
    ui->channelTable->setItem(row, 1, formatItem);
    ui->channelTable->setItem(row, 2, valueItem);
    ui->channelTable->setItem(row, 3, lowerRangeItem);
    ui->channelTable->setItem(row, 4, upperRangeItem);
    ui->channelTable->setItem(row, 5, colorItem);
    ui->channelTable->blockSignals(false);
}

void MainWindow::addChannelToPlot(RTTChannelSettings *settings){
    QCPGraph* graph;
    graph = ui->channelPlot->addGraph("time", settings->format);
    if (graph != nullptr) {
        graph->
    }
}

void MainWindow::addChannelSettings(RTTChannelSettings *settings) {
    channelSettingsList.append(settings);
    settings->id = idCount++;

    rttParser->addChannel(settings->id, settings->format);

    addChannelToTableView(settings);
}

void MainWindow::updateData(qint32 id, qint32 value) {
    for (int i = 0; i < ui->channelTable->rowCount(); ++i) {
        if (ui->channelTable->item(i,0)->text().toInt() == id) {
            ui->channelTable->item(i,2)->setText(QString::number(value));
        }
    }
}

void MainWindow::on_channelTable_cellChanged(int row, int column) {
    qint32 channelId = ui->channelTable->item(row, 0)->text().toInt();
    for (QList<RTTChannelSettings*>::iterator it = channelSettingsList.begin(); it != channelSettingsList.end(); it++) {
       if ((*it)->id == channelId) {
           if (column == 3) {
               (*it)->lowerRange = ui->channelTable->item(row,column)->text().toDouble();
           } else if (column == 4) {
               (*it)->upperRange = ui->channelTable->item(row,column)->text().toDouble();
           } else if (column == 5) {

           }
        }
   }
}
