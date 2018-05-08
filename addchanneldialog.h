#ifndef ADDCHANNELDIALOG_H
#define ADDCHANNELDIALOG_H

#include <QDialog>
#include <QPushButton>

class RTTChannelSettings;

namespace Ui {
class AddChannelDialog;
}

class AddChannelDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddChannelDialog(QWidget *parent = 0);
    ~AddChannelDialog();

signals:
    void addChannelSettings(RTTChannelSettings* settings);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::AddChannelDialog *ui;
};

#endif // ADDCHANNELDIALOG_H
