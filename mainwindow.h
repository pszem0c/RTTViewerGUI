#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class RTTConnector;
class RTTParser;
class RTTChannelSettings;
class RTTChannelController;
class RTTChannelLogger;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    RTTConnector* rttConnector;
    RTTParser* rttParser;
    RTTChannelController* rttChannelController;
    RTTChannelLogger* rttChannelLogger;

    void fillSerialPortBox();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_addChannelButton_clicked();
    void addChannelSettings(RTTChannelSettings* settings);
    void updateData(qint32 id, qint32 value);
    void updateData(QList<QPair<qint32, float> > dataList);
    void on_channelTable_cellChanged(int row, int column);
    void onRttConnect();
    void onRttDisconnect();
    void onRttError(QString);

    void on_action_Connect_triggered();

    void on_action_Disconnect_triggered();

    void on_action_Load_configuration_triggered();

    void on_action_Save_configuration_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
