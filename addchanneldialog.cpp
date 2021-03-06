#include "addchanneldialog.h"
#include "ui_addchanneldialog.h"
#include "rttchannelsettings.h"

AddChannelDialog::AddChannelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChannelDialog) {
    ui->setupUi(this);
}

AddChannelDialog::~AddChannelDialog() {
    delete ui;
}

void AddChannelDialog::on_buttonBox_clicked(QAbstractButton *button) {
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok) {
        RTTChannelSettings* channelSettings = new RTTChannelSettings;
        channelSettings->format = ui->formatLineEdit->text();
        channelSettings->lowerRange = ui->lowerRangeLineEdit->text().toDouble();
        channelSettings->upperRange = ui->upperRangeLineEdit->text().toDouble();
        channelSettings->color = ui->colorComboBox->currentText();
        channelSettings->msResolution = ui->msResolutionEdit->text().toInt();
        emit addChannelSettings(channelSettings);
    }
}
