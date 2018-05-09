#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class RTTConnector;
class RTTParser;
class RTTChannelSettings;
class RTTChannelController;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    RTTConnector* rttConnector;
    RTTParser* rttParser;
    RTTChannelController* rttChannelController;

    double plotTimeHorizon;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addChannelButton_clicked();
    void addChannelSettings(RTTChannelSettings* settings);
    void updateData(qint32 id, qint32 value);
    void on_channelTable_cellChanged(int row, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
