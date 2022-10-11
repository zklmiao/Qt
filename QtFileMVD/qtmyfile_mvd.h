#ifndef QTMYFILE_MVD_H
#define QTMYFILE_MVD_H

#include <QWidget>
#include <QApplication>

#include <QMap>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QSplitter>
#include <QDirIterator>
#include <QCryptographicHash>
#include <QDebug>

#include "renamedialog.h"
#include "exportdialog.h"
#include "filesystemmodel_checkbox.h"
#include "exportdetaildialog.h"

#define BASE_PATH "/home"
#define USERSPACE "/userName"
#define SAFESPACE "./"

namespace Ui {
class QtMyFile_MVD;
}

class QtMyFile_MVD : public QWidget
{
    Q_OBJECT

public:
    explicit QtMyFile_MVD(QWidget *parent = 0);
    ~QtMyFile_MVD();

    void setRootPath(const QString rootPath);

    // 初始化界面
    void initView(const QString defaultPath);

signals:
    void signalReturn();

    void signalNewUDiskJoinin(unsigned short UDISKID);
    void signalUdiskExit(unsigned short UDISKID);

public slots:
    // U盘事件
    // Udisk = 1
    // Udisk = 2
    // Udisk = 3
    // Udisk = 4
    // U盘连接
    void slotNewUDiskJoinin(unsigned short UDISKID);
    // U盘弹出
    void slotUdiskExit(unsigned short UDISKID);

private slots:
    // 单击选中文件夹 并展开该节点子树
    void on_treeView_clicked(const QModelIndex &index);
    // 单击选中
    void on_tableView_clicked(const QModelIndex &index);
    // 复制
    void on_pb_copy_clicked();
    // 粘贴
    void on_pb_paste_clicked();
    // 删除
    void on_pb_del_clicked();
    // 返回
    void on_pb_return_clicked();

    // 重命名
    void on_pb_rename_clicked();
    // 重命名实际操作
    void slotRename(QString);

    void on_pb_review_clicked();

    void on_pb_export_clicked();

    // 导出
    void cleanDir(const QString & path);
    void slotExport(unsigned t);

    void on_pb_newFolder_clicked();

    void on_pb_home_clicked();

private:
	// on : bool true : 使操作键钮面板处于可用状态。 false : 使操作键钮面板处于不可用状态。
	void onEdit(bool on = false);
		
	void showException(unsigned short ExcetionType);

    // 重置 复制粘贴按键状态
    void resetCpStatus();

	// 导出U盘检查
	bool checkUSB() const;

	unsigned int findNextID(const QList<unsigned long>& fileNameListNo);

    // 将勾选项 加入 操作队列
    void scanCheckedStatus(const QString srcPath);
    // 将导出项 加入 操作队列
    void scanExportItem(const QString srcPath);
    // 从操作队列中移除 保护资源 并警告
    void removeProtectSRC();
	// 获取 路径子串
	const QString getSectionPath(const QModelIndex index);
	// 非递归创建目录
	void makePath();

	// find a valid and near parent QModelindex.
	// Qt 46684 2276 QFileSystemWatcher:FindNextChangeNotification fail.
	const QModelIndex getNearParent(const QString path);
    // 导出实际操作
    int Export(QString src,QString dst);

private:
    Ui::QtMyFile_MVD *ui;

    QDir* _dir;
    QFile* _file;
    FileSystemModel_checkbox* _fileSystemModel;
    renameDialog *_renameDialog;
    ExportDialog *_exportDialog;
    exportDetailDialog *_exportDetail;

    // 部件分隔器 实现可拖拽定义窗口宽度。
    QSplitter *_splitter;

    // 源路径 点击 tableView 获取
    QString _srcPath;
    // 目标路径 treeView
    QString _dscPath;
    // 单击table单元选中的文件名
    QString _targetName;
    // 视图路径
    QString _defaultPath;
    QString _safePath;
    // 操作列
    QStringList _srcList;

    // true : joining
    // false : was exited...
    bool _udisk1Status = false;
    bool _udisk2Status = false;
    bool _udisk3Status = false;
    bool _udisk4Status = false;
    // true : this drive has joined.
    // false : this drive has exited.
    bool _udisk1Join = false;
    bool _udisk2Join = false;
    bool _udisk3Join = false;
    bool _udisk4Join = false;
};

#endif // QTMYFILE_MVD_H
