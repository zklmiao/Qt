#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

signals:
    // 1 screenshot 2 report 1+2 screenshot + report
    void sureExport(unsigned t);

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_pb_sure_clicked();

    void on_pb_cancel_clicked();

    void on_cb_scshot_clicked(bool checked);

    void on_cb_report_clicked(bool checked);

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
