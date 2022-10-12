#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


#define PAGE_RESOLUTION              300
#define MM_TO_PIXEL_FACTOR     25.4   //mm转化为像素比例因子

#define DISTRIBUTION_MAP_NULL_HEIGHT    1045
#define CROSS_MAP_NULL_HEIGHT                   1000
#define HISTOGRAM_MAP_NULL_HEIGHT       500

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TODO
//    ui->pushButton->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnOnPrint_clicked()
{
    qDebug() << "pdf view start";
    QPrinter printer(QPrinter::HighResolution);
    //自定义纸张大小
    printer.setPageSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(ui->centralwidget->height(), ui->centralwidget->width()),
                         QPrinter::Point);
    QPrintPreviewDialog *printPreview = nullptr;
    if (printPreview == nullptr)
    {
        printPreview = new QPrintPreviewDialog(&printer, this);
    }
    printPreview->setMinimumSize(1000,800);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), SLOT(SlotPrintView(QPrinter*)));
    printPreview->exec ();
    qDebug() << "pdf view end";
}

// 系统打印预览对话框
void MainWindow::SlotPrintView(QPrinter *printer)
{
//    qDebug() << "SlotPrintView start";
//    QString fileNameQs = "./testPdf.pdf";
//    QFile *pdfFile = new QFile(fileNameQs);
//    pdfFile->open(QIODevice::ReadWrite);
//    pdfWriter = new QPdfWriter(pdfFile);
////    pdfWriter->newPage();
//    pdfWriter->setPageSize(QPagedPaintDevice::A4);
//    pdfWriter->setResolution(100);
//    pdfWriter->setPageMargins(QMarginsF(0,0,0,0));//必须设置为0
//    pdfWriter->width();
//    pdfWriter->height();
//    if (pdfPainter == nullptr)
//    {

////        pdfPainter  = new QPainter(pdfWriter);
//        pdfPainter  = new QPainter(this);
//    }

//    pdfPainter->begin(printer);

//    QFont font;
//    QPen pen;
//    font.setPointSize(40);
//    pen.setColor(Qt::red);
//    pdfPainter->setFont(font);
//    pdfPainter->setPen(pen);
//    QTextOption option(Qt::AlignRight);
//    //    option.setWrapMode(QTextOption::WordWrap);

//    QString textString = "芯片编号：1234";
//    pdfPainter->drawText(500,1200, textString);
////    pdfPainter->drawText(QRect(0,0,2000,3000), textString, option);

//    //    CreatePdfPainter();

//    printer->newPage();
//    //获取界面的图片
//    QPixmap pixmap = this->grab();

//    QRect rect = pdfPainter->viewport();
//    int x = rect.width() / pixmap.width();
//    int y = rect.height() / pixmap.height();
//    //将图像(所有要画的东西)在pdf上按比例尺缩放
//    pdfPainter->scale(x, y);
//    //    CreatePdfPainter();
//    //画图
//    pdfPainter->drawPixmap(0, 40, pixmap);
//    pdfPainter->end();
//    pdfFile->flush();
//    pdfFile->close();
//    qDebug() << "SlotPrintView end";
}

// 页面绘制函数 同 protected: QPaint::paint(QpaintEvent* ev)
void MainWindow::SlotPreviewPdfOnWidget(QPrinter *printer)
{
    qDebug() << "SlotPreviewPdfOnWidget start";

//    printer->setOutputFormat(QPrinter::PdfFormat);
//    printer->setOutputFileName("./report.pdf");
    //自定义纸张大小,且需要在创建QPainter之前设置大小，否则会不起作用
    printer->setPaperSize(QPrinter::A4);//设置纸张大小
    //    printer->setPaperSize(QSizeF(2479,3508), QPrinter::Millimeter);//以mm单位设置
    printer->setMargins(QPagedPaintDevice::Margins{20,20,20,20});
    printer->setPageSize(QPrinter::A4);//设置页面大小
    printer->setResolution(300);//设置页面分别率大小，即按照A4，大小210x297mm 乘以分辨率300,得到页面已像素为单位的大小：2479x3058 pixel
    printer->setPageMargins(QMarginsF(0,0,0,0));//必须设置为0
    printer->setFullPage(true);//设置铺满整个纸张，报证在不同设备上，尤其是有打印机驱动的电脑上，若不设置为true，会默认留出留白部分，导致page的x,y会不是从(0,0)开始

    QFont font;
    QPen pen;

    QDateTime currentDataTime = QDateTime::currentDateTime();
    QString makeReportTimeString = currentDataTime.toString("yyyy-MM-dd hh:mm:ss ddd");

    QString reportLogoFileName = ":/pic/ProjectIcon.png";
    QPixmap reportAppLogoImage = QPixmap(reportLogoFileName);

    //    QPainter painter(printer);
//    QPainter *pdfPainter = new QPainter(printer);
    QPainter *pdfPainter = new QPainter();
    pdfPainter->begin(printer);

    //    QPrinter::PaperSize paperS = printer->paperSize();
    //    QPrinter::PageSize pageS = printer->pageSize();

    QRect pgRect = printer->pageRect();
    QSizeF ppRectS = printer->paperSize(QPrinter::Millimeter);

    //界面最终呈现的大小仅仅是pageSize的大小，不是设置的paperSize大小
    qDebug() << QString("pgRect: x %1, y %2, w: %3, h: %4").arg(pgRect.x()).arg(pgRect.y()).arg(pgRect.width()).arg(pgRect.height());;
    qDebug() << QString("ppRectS: w %1, h %2").arg(ppRectS.width()).arg(ppRectS.height());


    //当前Y方向的像素位置，开始，重置为0
    double currentYPosition = 0;


    //绘制抬头logo
    pdfPainter->drawPixmap(0,0, reportAppLogoImage.width()/2, reportAppLogoImage.height()/2, reportAppLogoImage);

    //Y方向累加logo所占据的高度
    currentYPosition += reportAppLogoImage.height()/2 + 4;

    //绘制抬头直线
    pen.setWidth(4);//设置画笔大小
    pen.setColor(Qt::blue);//设置画笔颜色
    pdfPainter->setPen(pen);
    pdfPainter->drawLine(0, currentYPosition, pgRect.width(), currentYPosition);


    //绘制前缀描述文字

    //设置后面文字字体和颜色，大小等
    font.setPointSize(6);
    font.setFamily("微软雅黑");
    pen.setColor(Qt::red);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    QTextOption option(Qt::AlignLeft);//文字对齐方式
    option.setWrapMode(QTextOption::WordWrap);

    double leftStartPixelPos = 40;//文字开始的位置
    double lineTextLineWidth = pgRect.width();//按照page尺寸最大宽度绘制，可以让文字充分显示
    double lineTextLineHeight = 40;//字体的高度
    double yoffet = 48;//每行位置的偏移量

    currentYPosition += 6;//Y方向累加高度，首行文字
    QString textString = "XX编号：" +  QString::fromLocal8Bit("1234");
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, lineTextLineWidth, lineTextLineHeight),
                         textString, option);

    currentYPosition += yoffet;//Y方向累加高度
    textString = "XX信息：" +  QString::fromLocal8Bit("TEST");
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, lineTextLineWidth, lineTextLineHeight),
                         textString, option);

    currentYPosition += yoffet;//Y方向累加高度
    textString = "软件版本：" +  QString::fromLocal8Bit("TV101");
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, lineTextLineWidth, lineTextLineHeight),
                         textString, option);


    //绘制标题文字
    font.setPointSize(20);
    pen.setColor(Qt::darkGreen);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    option.setAlignment(Qt::AlignCenter);

    double titleTextLineWidth = pgRect.width();//按照page尺寸最大宽度绘制，可以让文字充分显示
    double titleTextLineHeight = 80;//字体的高度

    currentYPosition += 80;
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, titleTextLineWidth, titleTextLineHeight),
                         QString("XXXXX分析报告"), option);

    //绘制1级标题文字
    font.setPointSize(16);
    pen.setColor(Qt::darkBlue);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    option.setAlignment(Qt::AlignLeft);

    double level1TextLineWidth = pgRect.width();//按照page尺寸最大宽度绘制，可以让文字充分显示
    double level1TextLineHeight = 80;//字体的高度

    currentYPosition += 100;
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, level1TextLineWidth, level1TextLineHeight),
                         QString("1. 示例图片1"), option);


    //画图
    //从(0,0)位置开始绘制，同时大小按照原图大小的width(),height()的各一半大小，进行缩放绘制
    //若想按照缩小的1/3绘制，采用width()/3,height()/3即可
    QPixmap mapbg = QPixmap(":/pic/bg3.jpg");
    currentYPosition += 120;
    pdfPainter->drawPixmap(leftStartPixelPos, currentYPosition, mapbg.width()/2, mapbg.height()/2,mapbg);

    currentYPosition += mapbg.height()/2 + 60;
    mapbg = QPixmap(":/pic/bg4.jpg");
    pdfPainter->drawPixmap(leftStartPixelPos, currentYPosition, mapbg.width()/2, mapbg.height()/2,mapbg);


    //页面底端绘制时间戳
    font.setPointSize(12);
    pen.setColor(Qt::darkGray);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    option.setAlignment(Qt::AlignLeft);
    currentYPosition = pgRect.height() - 80;//本页最下方，左下角绘制时间戳
    lineTextLineHeight = 60;
    textString = makeReportTimeString;
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, lineTextLineWidth, lineTextLineHeight),
                         textString, option);

    //新建立一页
    printer->newPage();
    currentYPosition = 0;//新的一页，重置为0

    //绘制页面背景
    QPixmap pgbg = QPixmap(":/pic/pgbg.jpg");
    //使背景图充满整个页面，从(0,0)坐标开始，宽和高，采用页面大小
    pdfPainter->drawPixmap(0, 0, pgRect.width(), pgRect.height(),pgbg);

    //新建立一页
    currentYPosition = 0;//新的一页，重置为0
    printer->newPage();

    //绘制页面背景
    //使背景图充满整个页面，从(0,0)坐标开始，宽和高，采用页面大小
    pdfPainter->drawPixmap(0, 0, pgRect.width(), pgRect.height(),pgbg);


    //绘制1级标题文字
    font.setPointSize(16);
    pen.setColor(Qt::darkBlue);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    option.setAlignment(Qt::AlignLeft);

    currentYPosition += 80;
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, level1TextLineWidth, level1TextLineHeight),
                         QString("2. 示例图片2"), option);

    //获取界面的图片
    QPixmap pixmap = this->grab();

    //画图: 一张图剧中显示
    //根据pageSize，即页面大小，和图像大小，自动剧中显示
    currentYPosition += 140;
    double pixMapLeftStartPos = 0;
    double scaleFactor = 0;

    double leftRightMargin = 40;//左右2边需要流出的空白部分
    double mapViewWidth = pgRect.width() - leftRightMargin * 2;//计算图像流出空白后所需要的宽度，即显示的宽度

    scaleFactor = static_cast<double>(pixmap.width())/ mapViewWidth;//计算图像宽度，所占据页面宽度的比例
    if (scaleFactor <= 0)
    {
        scaleFactor = 1;//防止为0
    }
    double scaledMapWidth = static_cast<double>(pixmap.width())/scaleFactor;//按照计算比例，得出图像需要展示的绘制宽度
    if (pgRect.width() > scaledMapWidth)//图像显示宽度，小于页面宽度时，计算图像绘制的x方向起始位置
    {
        //换算图像X方向起始位置，按照剧中显示，页面一半的宽度减去图像绘制宽度的一半，即图像X方向的起始坐标点
        pixMapLeftStartPos = (pgRect.width() -  scaledMapWidth) * 0.5f;//计算图像左上角的x起点//其实算出来就是等于左边的留白值
    }
    else
    {//若图像大小和页面宽度相等，或者图像宽度更宽时，从0开始
        pixMapLeftStartPos = 0;
    }
    pdfPainter->drawPixmap(pixMapLeftStartPos, currentYPosition, scaledMapWidth, pixmap.height()/scaleFactor,pixmap);//绘制图像

    //2张图，并列且剧中显示
    QPixmap pixmap1 = QPixmap(":/pic/bg1.jpg");
    QPixmap pixmap2 = QPixmap(":/pic/bg2.jpg");
    currentYPosition += pixmap.height()/scaleFactor + 40;

    double midSapce = 60;//2图之间的留白
    mapViewWidth = (pgRect.width() - leftRightMargin * 2);//页面总宽度，减去2边留白
    mapViewWidth = (mapViewWidth - midSapce) * 0.5f;//减去2图之间的留白，再除以2，为2个图，各图的绘图显示宽度
    double bg1Factor = static_cast<double>(pixmap1.width()) / mapViewWidth;
    double bg2Factor = static_cast<double>(pixmap2.width()) / mapViewWidth;
    if (bg1Factor <= 0)
    {
        bg1Factor = 1;//防止为0
    }
    if (bg2Factor <= 0)
    {
        bg2Factor = 1;//防止为0
    }
    double scaledMapWidth1 = static_cast<double>(pixmap1.width())/bg1Factor;//按照计算比例，得出图像需要展示的绘制宽度
    double scaledMapWidth2 = static_cast<double>(pixmap2.width())/bg2Factor;//按照计算比例，得出图像需要展示的绘制宽度
    double pageMidline = pgRect.width()/2;


    if (pageMidline > scaledMapWidth1)//图像显示宽度，小于页面宽度时，计算图像绘制的x方向起始位置
    {
        //换算图像X方向起始位置，按照剧中显示，页面一半的宽度减去图像绘制宽度的一半，即图像X方向的起始坐标点
        pixMapLeftStartPos = (pageMidline -  scaledMapWidth1) * 0.5f;//计算图像左上角的x起点
    }
    else
    {//若图像大小和页面宽度相等，或者图像宽度更宽时，从0开始
        pixMapLeftStartPos = 0;
    }
    pdfPainter->drawPixmap(pixMapLeftStartPos, currentYPosition, scaledMapWidth1, pixmap1.height()/bg1Factor,pixmap1);//绘制图像1

    if (pageMidline > scaledMapWidth2)//图像显示宽度，小于页面宽度时，计算图像绘制的x方向起始位置
    {
        //换算图像X方向起始位置，按照剧中显示，页面一半的宽度减去图像绘制宽度的一半，即图像X方向的起始坐标点
        pixMapLeftStartPos = pageMidline + (pageMidline -  scaledMapWidth1) * 0.5f;//计算图像左上角的x起点
    }
    else
    {//若图像大小和页面宽度相等，或者图像宽度更宽时，从0开始
        pixMapLeftStartPos = pageMidline;
    }
    pdfPainter->drawPixmap(pixMapLeftStartPos, currentYPosition, scaledMapWidth2, pixmap2.height()/bg2Factor,pixmap2);//绘制图像2

    //页面底端绘制时间戳
    font.setPointSize(12);
    pen.setColor(Qt::darkGray);
    pdfPainter->setFont(font);
    pdfPainter->setPen(pen);
    option.setAlignment(Qt::AlignLeft);
    currentYPosition = pgRect.height() - 80;//本页最下方，左下角绘制时间戳
    lineTextLineHeight = 60;
    textString = makeReportTimeString;
    pdfPainter->drawText(QRect(leftStartPixelPos, currentYPosition, lineTextLineWidth, lineTextLineHeight),
                         textString, option);
    //结束绘制
    pdfPainter->end();

    qDebug() << "SlotPreviewPdfOnWidget end";
}

void MainWindow::CreatePdfPainter()
{
    //    QFont font;
    //    QPen pen;
    //    font.setPointSize(20);
    //    pen.setColor(Qt::red);
    //    pdfPainter->setFont(font);
    //    pdfPainter->setPen(pen);
    //    QTextOption option(Qt::AlignLeft);
    //    option.setWrapMode(QTextOption::WordWrap);


    //    QString textString = "芯片编号：1234";
    //    pdfPainter->drawText(10,120, textString);
    //    //    pdfPainter->drawText(QRect(0,0, 500, 1000), textString, option);


    //    //获取界面的图片
    //    QPixmap pixmap = this->grab();

    //    QRect rect = pdfPainter->viewport();
    //    int x = rect.width() / pixmap.width();
    //    int y = rect.height() / pixmap.height();
    //    //将图像(所有要画的东西)在pdf上按比例尺缩放
    //    pdfPainter->scale(x, y);
    //    //画图
    //    pdfPainter->drawPixmap(0, 40, pixmap);
}

// PDF 预览
void MainWindow::on_BtnWidgetPDF_clicked()
{
    // 构造 打印机绘图对象
    if (pdfPrinter == nullptr)
    {
        pdfPrinter = new QPrinter();
    }

    // 清理 旧内容
    if (printPreviewWidget != nullptr)
    {
        QLayoutItem *item = nullptr;
        while((item = ui->verticalLayout->takeAt(0)) != nullptr)//清空layout元素
        {
            delete item;
        }

        delete printPreviewWidget;
        printPreviewWidget = nullptr;//需要删除后，再创建，才能刷新，触发repaint
    }
    // 构造PDF生成后预览界面
    if (printPreviewWidget == nullptr)
    {
        printPreviewWidget = new QPrintPreviewWidget(pdfPrinter,this);
        printPreviewWidget->setZoomMode(QPrintPreviewWidget::FitInView);//FitInView：根据视野大小，自动缩放，会完整显示一页；FitToWidth：根据宽度缩放，会无法完整显示一页
        //printPreviewWidget->setSinglePageViewMode();//单页显示
        printPreviewWidget->setViewMode(QPrintPreviewWidget::ViewMode::FacingPagesView);//对开显示
        printPreviewWidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        ui->verticalLayout->addWidget(printPreviewWidget);
    }
    //构造成功 连接绘制信号
    connect(printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(SlotPreviewPdfOnWidget(QPrinter*)));
    // 发送绘制 信号 同 QPaint::update() 信号 机制
    printPreviewWidget->updatePreview();//出发一次，更新preview
}


void MainWindow::on_BtnZoomOut_clicked()
{
    qDebug() << "ZoomOut";
    printPreviewWidget->zoomIn();
    QRect pgRect = pdfPrinter->pageRect();
    QSizeF ppRectS = pdfPrinter->paperSize(QPrinter::Point);

    //界面最终呈现的大小仅仅是pageSize的大小，不是设置的paperSize大小
    qDebug() << QString("ZoomOut pgRect: x %1, y %2, w: %3, h: %4").arg(pgRect.x()).arg(pgRect.y()).arg(pgRect.width()).arg(pgRect.height());;
    qDebug() << QString("ZoomOut ppRectS: w %1, h %2").arg(ppRectS.width()).arg(ppRectS.height());

}


void MainWindow::on_BtnZoomIn_clicked()
{
    qDebug() << "ZoomIn";
    printPreviewWidget->zoomOut();

    QRect pgRect = pdfPrinter->pageRect();
    QSizeF ppRectS = pdfPrinter->paperSize(QPrinter::Point);

    //界面最终呈现的大小仅仅是pageSize的大小，不是设置的paperSize大小
    qDebug() << QString("ZoomIn pgRect: x %1, y %2, w: %3, h: %4").arg(pgRect.x()).arg(pgRect.y()).arg(pgRect.width()).arg(pgRect.height());;
    qDebug() << QString("ZoomIn ppRectS: w %1, h %2").arg(ppRectS.width()).arg(ppRectS.height());

}


void MainWindow::on_BtnNextPage_clicked()
{
    qDebug() << "on_BtnNextPage_clicked";
    int currentPageIndex = printPreviewWidget->currentPage();
    if (currentPageIndex < printPreviewWidget->pageCount())
    {
        currentPageIndex++;
        printPreviewWidget->setCurrentPage(currentPageIndex);
    }
}


void MainWindow::on_BtnPrevPage_clicked()
{
    qDebug() << "on_BtnPrevPage_clicked";
    int currentPageIndex = printPreviewWidget->currentPage();
    if (currentPageIndex > 1)
    {
        currentPageIndex--;
        printPreviewWidget->setCurrentPage(currentPageIndex);
    }
}


void MainWindow::on_BtnFirstPage_clicked()
{
    qDebug() << "on_BtnFirstPage_clicked";
    int currentPageIndex = 1;
    printPreviewWidget->setCurrentPage(currentPageIndex);
}
