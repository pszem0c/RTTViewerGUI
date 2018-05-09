#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../RTTViewer/rttconnector.h"
#include "../RTTViewer/rttparser.h"
#include "addchanneldialog.h"
#include "rttchannelsettings.h"
#include "qcustomplot.h"
#include "rttchannelcontroller.h"

#include <QStringList>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    rttConnector = new RTTConnector();
    rttParser = new RTTParser();
    rttChannelController = new RTTChannelController(ui->channelTable, ui->channelPlot);
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

void MainWindow::addChannelSettings(RTTChannelSettings *settings) {
    rttParser->addChannel(settings->id, settings->format);

    rttChannelController->addChannelToTable(settings);
}

void MainWindow::updateData(qint32 id, qint32 value) {
    rttChannelController->updateChannelValue(id, value);
}

void MainWindow::on_channelTable_cellChanged(int row, int column) {
    rttChannelController->tableValueChanged(row, column);
}
