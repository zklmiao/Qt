#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPdfWriter>
#include <QFile>
#include <QTextOption>
#include <QDateTime>
#include <memory>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QPrintPreviewWidget>
#include <QPrintPreviewDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 系统打印预览对话框
    void SlotPrintView(QPrinter *printer);
    // 生成PDF预览
    void SlotPreviewPdfOnWidget(QPrinter *printer);

    // 打印PDF
    void on_btnOnPrint_clicked();
    // PDF 预览
    void on_BtnWidgetPDF_clicked();
    // 放大
    void on_BtnZoomOut_clicked();
    // 缩小
    void on_BtnZoomIn_clicked();

    // 下一页
    void on_BtnNextPage_clicked();
    // 上一页
    void on_BtnPrevPage_clicked();
    // 首页
    void on_BtnFirstPage_clicked();


private:
    Ui::MainWindow *ui;
    QPrintPreviewWidget *printPreviewWidget = nullptr;//打印预览控件
    QPrinter *pdfPrinter = nullptr;//pdf绘图printer

    // 系统打印对话框
    QPdfWriter* pdfWriter = nullptr;
    QPainter* pdfPainter  = nullptr;
    void CreatePdfPainter();


};
#endif // MAINWINDOW_H
