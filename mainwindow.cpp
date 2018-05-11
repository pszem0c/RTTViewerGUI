#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../RTTViewer/rttconnector.h"
#include "../RTTViewer/rttparser.h"
#include "addchanneldialog.h"
#include "rttchannelsettings.h"
#include "qcustomplot.h"
#include "rttchannelcontroller.h"
#include "rttchannellogger.h"

#include <QStringList>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    rttConnector = new RTTConnector();
    rttParser = new RTTParser();
    rttChannelController = new RTTChannelController(ui->channelTable, ui->channelPlot);
    rttChannelLogger = new RTTChannelLogger();

    connect(rttConnector, SIGNAL(lineRead(QString)), rttParser, SLOT(parseSlot(QString)));
    //connect(rttParser, SIGNAL(lineParsed(qint32,qint32)),this, SLOT(updateData(qint32,qint32)));
    connect(rttParser, SIGNAL(lineParsed(QList<QPair<qint32, qint32> >)),this, SLOT(updateData(QList<QPair<qint32, qint32> >)));

    rttConnector->connectToHost();

    ui->channelTable->setColumnCount(6);
    ui->channelTable->setRowCount(0);
    ui->channelTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Format" << "Value" << "Lower range" << "Upper range" << "Color");
}

MainWindow::~MainWindow() {
    rttConnector->disconnectFromHost();
    delete rttChannelLogger;
    delete rttConnector;
    delete rttParser;
    delete rttChannelController;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    rttConnector->disconnectFromHost();
    event->accept();
}


void MainWindow::on_addChannelButton_clicked() {
    AddChannelDialog* addChannelDialog = new AddChannelDialog(this);
    connect(addChannelDialog, SIGNAL(addChannelSettings(RTTChannelSettings*)), this, SLOT(addChannelSettings(RTTChannelSettings*)));
    addChannelDialog->exec();
}

void MainWindow::addChannelSettings(RTTChannelSettings *settings) {
    rttChannelController->addChannel(settings);
    rttParser->addChannel(settings->id, settings->format);
    rttChannelLogger->addChannel(settings);
}

void MainWindow::updateData(qint32 id, qint32 value) {
    rttChannelController->updateChannelValue(id, value);
}

void MainWindow::updateData(QList<QPair<qint32, qint32> > dataList) {
    rttChannelController->updateChannels(dataList);
    rttChannelLogger->logData(dataList);
}

void MainWindow::on_channelTable_cellChanged(int row, int column) {
    rttChannelController->tableValueChanged(row, column);
}
