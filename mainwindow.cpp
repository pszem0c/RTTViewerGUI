#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../RTTViewer/rttconnector.h"
#include "../RTTViewer/rttparser.h"
#include "addchanneldialog.h"
#include "rttchannelsettings.h"
#include "qcustomplot.h"
#include "rttchannelcontroller.h"
#include "rttchannellogger.h"

#include <QtSerialPort/QSerialPortInfo>
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
    connect(rttParser, SIGNAL(lineParsed(QList<QPair<qint32, float> >)),this, SLOT(updateData(QList<QPair<qint32, float> >)));

    connect(rttConnector, SIGNAL(connectedToHost()), this, SLOT(onRttConnect()));
    connect(rttConnector, SIGNAL(disconnectedFromHost()), this, SLOT(onRttDisconnect()));
    connect(rttConnector, SIGNAL(socketError(QString)), this, SLOT(onRttError(QString)));

    //rttConnector->connectToHost();

    ui->channelTable->setColumnCount(6);
    ui->channelTable->setRowCount(0);
    ui->channelTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Format" << "Value" << "Lower range" << "Upper range" << "Color");

    fillSerialPortBox();
    rttChannelController->loadConfigurationFromFile("def.conf");
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


void MainWindow::fillSerialPortBox() {
    ui->serialPortBox->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui->serialPortBox->addItem(info.portName(), info.portName());
    }
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

void MainWindow::updateData(QList<QPair<qint32, float> > dataList) {
    rttChannelController->updateChannels(dataList);
    rttChannelLogger->logData(dataList);
}

void MainWindow::on_channelTable_cellChanged(int row, int column) {
    rttChannelController->tableValueChanged(row, column);
}

void MainWindow::onRttConnect() {
    ui->statusLabel->setText("Connected!");
}

void MainWindow::onRttDisconnect() {
    ui->statusLabel->setText("Disconnected!");
}

void MainWindow::onRttError(QString err) {
    ui->statusLabel->setText(QString("Error: " + err));
}

void MainWindow::on_action_Connect_triggered() {
    rttConnector->connectToHost(ui->serialPortBox->currentText());
}

void MainWindow::on_action_Disconnect_triggered() {
    rttConnector->disconnectFromHost();
}

void MainWindow::on_action_Load_configuration_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load configuration"));
    QList<RTTChannelSettings*> list = rttChannelController->loadConfigurationFromFile(fileName);
    for (QList<RTTChannelSettings*>::iterator it = list.begin(); it != list.end(); it++) {
        rttParser->addChannel((*it)->id, (*it)->format);
        rttChannelLogger->addChannel(*it);
    }
}

void MainWindow::on_action_Save_configuration_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save configuration"));
    rttChannelController->saveConfigurationToFile(fileName);
}

void MainWindow::on_pushButton_clicked() {
    fillSerialPortBox();
}
