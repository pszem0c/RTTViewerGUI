#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../RTTViewer/rttconnector.h"
#include "../RTTViewer/rttparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    rttConnector = new RTTConnector();
    rttParser = new RTTParser();

    connect(rttConnector, SIGNAL(lineRead(QString)), rttParser, SLOT(parseSlot(QString)));
    rttConnector->connectToHost();
    ui->channelTabWidget->removeTab(1);
}

MainWindow::~MainWindow() {
    rttConnector->disconnectFromHost();
    delete rttConnector;
    delete rttParser;
    delete ui;
}

