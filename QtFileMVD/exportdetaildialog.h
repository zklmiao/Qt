#ifndef EXPORTDETAILDIALOG_H
#define EXPORTDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class exportDetailDialog;
}

class exportDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit exportDetailDialog(QWidget *parent = 0);
    ~exportDetailDialog();
    void appen(const QString str,int color);
    void hideLable();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_pb_close_clicked();

private:
    Ui::exportDetailDialog *ui;
};

#endif // EXPORTDETAILDIALOG_H
