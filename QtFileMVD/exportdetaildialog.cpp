#include "exportdetaildialog.h"
#include "ui_exportdetaildialog.h"

exportDetailDialog::exportDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exportDetailDialog)
{
    ui->setupUi(this);
    setGeometry(460/2, 275/2, 360, 330);
}

exportDetailDialog::~exportDetailDialog()
{
    delete ui;
}

void exportDetailDialog::hideLable()
{
    ui->label->hide();
}

void exportDetailDialog::appen(const QString str,int color)
{
    QString strcolor = "black";
    switch (color) {
    case 0:
    break;
    case 1:
        strcolor = "green";
    break;
    case -1:
        strcolor = "red";
    break;
    }
    QString section = QString("<font color=%2>%1</font>").arg(str).arg(strcolor);
    ui->textBrowser->append(section);
}

void exportDetailDialog::showEvent(QShowEvent *event)
{
    ui->label->show();
    ui->label->setText(tr("正在导出...请勿操作!"));
}

void exportDetailDialog::closeEvent(QCloseEvent * event)
{
    Q_UNUSED(event)
    ui->textBrowser->clear();
}

void exportDetailDialog::on_pb_close_clicked()
{
    close();
}
