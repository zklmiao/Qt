#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    setGeometry(400/2, 275/2, 320, 240);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::showEvent(QShowEvent * event)
{
    if(ui->cb_scshot->isChecked() || ui->cb_report->isChecked())
        ui->pb_sure->setDisabled(false);
    else
        ui->pb_sure->setDisabled(true);
}

void ExportDialog::on_pb_sure_clicked()
{
    int exportType = 0;
    if(ui->cb_scshot->isChecked())
        exportType += 1;
    if(ui->cb_report->isChecked())
        exportType += 2;
    emit sureExport(exportType);
    close();
}

void ExportDialog::on_pb_cancel_clicked()
{
    close();
}

void ExportDialog::on_cb_scshot_clicked(bool checked)
{
    if(checked || ui->cb_report->isChecked())
        ui->pb_sure->setDisabled(false);
    else
        ui->pb_sure->setDisabled(true);
}

void ExportDialog::on_cb_report_clicked(bool checked)
{
    if(checked || ui->cb_scshot->isChecked())
        ui->pb_sure->setDisabled(false);
    else
        ui->pb_sure->setDisabled(true);
}
