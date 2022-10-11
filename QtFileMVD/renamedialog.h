#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class renameDialog;
}

class renameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit renameDialog(QWidget *parent = 0);
    ~renameDialog();

    void setName(const QString name);

signals:
    void sureRename(QString);

private slots:
    void on_pb_sure_clicked();

    void on_pb_chanl_clicked();

    void slotSetLineEditText(QString);

private:
    Ui::renameDialog *ui;
};

#endif // RENAMEDIALOG_H
