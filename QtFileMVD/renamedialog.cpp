#include "renamedialog.h"
#include "ui_renamedialog.h"

renameDialog::renameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::renameDialog)
{
    ui->setupUi(this);
    setGeometry(400/2, 275/2, 320, 240);

    //set soft keypad type
//    ui->lineEdit->setProperty("Type", Keypad_Type::English);
}

renameDialog::~renameDialog()
{
    delete ui;
}

void renameDialog::setName(const QString name)
{
    ui->lineEdit->setText(name);
}


void renameDialog::on_pb_sure_clicked()
{
    emit sureRename(ui->lineEdit->text());
    window()->close();
}

void renameDialog::on_pb_chanl_clicked()
{
    window()->close();
}

void renameDialog::slotSetLineEditText(QString text)
{
    ui->lineEdit->setText(text);
}
