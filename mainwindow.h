#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RTTConnector;
class RTTParser;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    RTTConnector* rttConnector;
    RTTParser* rttParser;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
