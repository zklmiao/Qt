#include "qtmyfile_mvd.h"
#include "ui_qtmyfile_mvd.h"

QtMyFile_MVD::QtMyFile_MVD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtMyFile_MVD)
{
    ui->setupUi(this);

    this->setFont(QFont("Microsoft Yahei", 9));

    ui->treeView->setStyleSheet("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:20px;height:20px;}");
    ui->tableView->setStyleSheet("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:20px;height:20px;}");

    ui->pb_export->hide();

    _dir = new QDir();
    _file = new QFile();
    _fileSystemModel = new FileSystemModel_checkbox;
    _renameDialog = new renameDialog(this);
    _exportDialog = new ExportDialog(this);
    _exportDetail = new exportDetailDialog(this);

    onEdit();

    _srcPath.clear();
    _dscPath.clear();
    _targetName.clear();

    // 设置列宽
    ui->treeView->header()->resizeSection(0,500);
    ui->treeView->header()->hide();
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);    

    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);

    // 部件分隔器 实现可拖拽定义窗口宽度。
    _splitter = new QSplitter(this);
    _splitter->addWidget(ui->treeView);
    _splitter->addWidget(ui->tableView);
    _splitter->setFixedSize(QSize(680,560));
    _splitter->show();

    connect(_renameDialog,SIGNAL(sureRename(QString)),this,SLOT(slotRename(QString)));
    setRootPath("./");
    initView("./");
}

QtMyFile_MVD::~QtMyFile_MVD()
{
    delete ui;

    _srcPath.clear();
    _dscPath.clear();
    _targetName.clear();

    if(_renameDialog)
        delete _renameDialog;

    if(_dir) delete _dir;
    _dir = nullptr;

    if(_file) delete _file;
    _file = nullptr;

    if(_fileSystemModel) delete _fileSystemModel;
    _fileSystemModel = nullptr;
}

void QtMyFile_MVD::setRootPath(const QString rootPath)
{
    _defaultPath = rootPath;
    _safePath = SAFESPACE;
}

// 初始化界面
void QtMyFile_MVD::initView(const QString defaultPath)
{
    _fileSystemModel->setRootPath(defaultPath);

    ui->treeView->setModel(_fileSystemModel);
    ui->tableView->setModel(_fileSystemModel);

    ui->treeView->setRootIndex(_fileSystemModel->index(defaultPath));
    ui->tableView->setRootIndex(_fileSystemModel->index(defaultPath));

    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionModel(ui->treeView->selectionModel());

    QObject::connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
                     ui->tableView,SLOT(setRootIndex(QModelIndex)));
    QObject::connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
                     ui->treeView,SLOT(expand(QModelIndex)));

    // 标题头自适应大小
    ui->treeView->header()->adjustSize();
    // 设置列宽
    ui->treeView->header()->resizeSection(0,500);
    ui->treeView->header()->setDefaultAlignment(Qt::AlignLeft);
    ui->treeView->header()->hide();

    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);

    ui->tableView->setColumnWidth(0,300);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    _srcPath.clear();
    _dscPath.clear();
    _targetName.clear();

    ui->treeView->collapseAll();
    ui->treeView->expand(_fileSystemModel->index(defaultPath));
}

// 单击选中文件夹 并展开该节点子树
void QtMyFile_MVD::on_treeView_clicked(const QModelIndex &index)
{
    // 鼠标点击目录树节点的目录名称
    _dscPath = _fileSystemModel->filePath(index);
    if(!_fileSystemModel->isDir(index))
        _dscPath = _fileSystemModel->fileInfo(index).absolutePath();

    ui->pb_rename->setDisabled(true);

//    qDebug() << __func__ << "dsc:" << _dscPath;
}

// 单击选中
void QtMyFile_MVD::on_tableView_clicked(const QModelIndex &index)
{
    _srcPath = _fileSystemModel->filePath(index);

    // 拿到 选中单元的 文件/文件夹信息
    _targetName = index.parent().child(index.row(),0).data().toString();

    // 有 无效 索引风险
//    if(ui->treeView->currentIndex().data(Qt::CheckStateRole).toInt())
//        _fileSystemModel->setData(ui->treeView->currentIndex(),Qt::CheckState::Unchecked,Qt::ItemDataRole::CheckStateRole);
//    else
//        _fileSystemModel->setData(ui->treeView->currentIndex(),Qt::CheckState::Checked,Qt::ItemDataRole::CheckStateRole);

//    qDebug() << __func__ << "row" << index.row()
//             << _targetName << _srcPath;

    onEdit(true);

    ui->pb_copy->setDisabled(false);
    ui->pb_paste->setDisabled(true);

    // 居中显示 点击到 的目录树子项
    ui->treeView->scrollTo(index.parent().child(index.row(),0));
}

// 复制 找到勾选的对象
void QtMyFile_MVD::on_pb_copy_clicked()
{
    qDebug() << "copy src:" << _srcPath;
    scanCheckedStatus(_defaultPath);
    ui->pb_copy->setDisabled(true);
    ui->pb_paste->setDisabled(false);
}

// 粘贴
void QtMyFile_MVD::on_pb_paste_clicked()
{
    if(_srcPath.isEmpty() || _srcPath.isNull() || _srcPath == QString("/"))
	{
		showException(1);
        resetCpStatus();
		return;
	}

    if(_dscPath.isEmpty() || _dscPath.isNull() || _dscPath == QString("/"))
    {
		showException(1);
        resetCpStatus();
        return;
	}
    if(_srcList.empty())
    {
        showException(1);
        resetCpStatus();
        return;
	}

//    qDebug() << "paste dsc:" << _dscPath;

    makePath();

    if(_dir->exists(_dscPath))
    {
        for(QString i : _srcList)
        {
            QModelIndex index = _fileSystemModel->index(i);
            if(index.isValid())
            {
                // cp file
                if(!_fileSystemModel->isDir(index))
                {
					// 拿到路径 子串
					QString sectionPath = getSectionPath(index);
					QString path = _dscPath + QString("/") + sectionPath;
					_file->copy(i,path);
					//qDebug() << sectionPath << "\n" << path;
                }
            }
        }
    }
    _srcList.clear();
    _srcPath.clear();
    system("sync");
    ui->pb_copy->setDisabled(false);
    ui->pb_paste->setDisabled(true);
}

// 删除
void QtMyFile_MVD::on_pb_del_clicked()
{
    if(_srcPath.isEmpty() || _srcPath.isNull())
        return;
    scanCheckedStatus(_defaultPath);
    removeProtectSRC();
    if(_srcList.empty()) return;

//	const QModelIndex safeIndex = getNearParent(_srcList.first());

    // 颠倒一下 先删文件 再删文件夹
    std::reverse(_srcList.begin(),_srcList.end());

    if(ui->treeView->currentIndex().isValid())
    {
        for(QString const & i : _srcList)
        {
            if(_fileSystemModel->index(i).isValid())
            {
                if(!_fileSystemModel->isDir(_fileSystemModel->index(i)))
                    _fileSystemModel->remove(_fileSystemModel->index(i));
                else
                    _fileSystemModel->rmdir(_fileSystemModel->index(i));
            }
        }
    }
    _srcList.clear();
	//ui->tableView->setRootIndex(safeIndex);
    ui->tableView->setRootIndex(_fileSystemModel->index(_safePath));
}

// on : bool true : 使操作键钮面板处于可用状态。 false : 使操作键钮面板处于不可用状态。
void QtMyFile_MVD::onEdit(bool on)
{
    if(on)
    {
        ui->pb_review->setDisabled(false);
        ui->pb_copy->setDisabled(false);
        ui->pb_paste->setDisabled(false);
        ui->pb_del->setDisabled(false);
        ui->pb_return->setDisabled(false);
        ui->pb_rename->setDisabled(false);
    }
    else
    {
        ui->pb_review->setDisabled(true);
        ui->pb_copy->setDisabled(true);
        ui->pb_paste->setDisabled(true);
        ui->pb_del->setDisabled(true);
        ui->pb_rename->setDisabled(true);
        ui->pb_return->setDisabled(false);
    }
}

void QtMyFile_MVD::showException(unsigned short ExcetionType)
{
	QString str;
	switch (ExcetionType)
	{
	case 1: 
		str = "操作出错，请重新操作.";
	break;
    }

    /* show message box */
    /* code */
}

void QtMyFile_MVD::resetCpStatus()
{
    ui->pb_copy->setDisabled(false);
    ui->pb_paste->setDisabled(true);
}

// 返回 并显示主界面键钮
void QtMyFile_MVD::on_pb_return_clicked()
{
    emit signalReturn();
    onEdit();
    window()->close();
}

void QtMyFile_MVD::slotRename(QString name)
{
    if(_targetName == name) return ;
    if(name.isEmpty() || name.isNull())
    {
        return ;
    }

    QFileInfo info(_srcPath);
    if(info.isFile())
    {
        // 文件重命名 默认文件格式
        QString parentPath;
        QString suffix;
        QFile file(_srcPath);
        QFileInfo info(file);
        suffix = info.completeSuffix();
        parentPath = info.absoluteDir().absolutePath();

        file.rename(parentPath +"/"+ name + "." + suffix);
    }
    else if(info.isDir())
    {
        _dir->setPath(_srcPath);
        _dir->cdUp();
        QString oldname = info.fileName();
        _dir->rename(oldname,name);
    }
}

void QtMyFile_MVD::on_pb_rename_clicked()
{
    scanCheckedStatus(_defaultPath);
    for(int i = 0; i < _srcList.size(); ++ i)
    {
        if(_fileSystemModel->index(_srcList[i]).isValid())
            _fileSystemModel->setData(_fileSystemModel->index(_srcList[i]),Qt::CheckState::Unchecked,Qt::ItemDataRole::CheckStateRole);
    }
    if(ui->treeView->currentIndex().isValid())
        _fileSystemModel->setData(ui->treeView->currentIndex(),Qt::CheckState::Checked,Qt::ItemDataRole::CheckStateRole);

    if(ui->treeView->currentIndex().parent().isValid()
            && ui->treeView->currentIndex().parent().child(ui->treeView->currentIndex().row(), 0).isValid())
        _targetName = ui->treeView->currentIndex().parent()
            .child(ui->treeView->currentIndex().row(), 0).data(Qt::DisplayRole).toString();
    _srcPath = _fileSystemModel->filePath(ui->treeView->currentIndex());

    QFileInfo info(_srcPath);
    if(info.absoluteFilePath() != QString("/需要保护的路径...")
            && info.absoluteFilePath() != QString("/")
            && info.absoluteFilePath() != QString("/protect.res")
            && info.absoluteFilePath() != QString("/mnt/udisk1")
            && info.absoluteFilePath() != QString("/需要保护的资源..."))
    {
        _renameDialog->setName(tr(info.completeBaseName().toStdString().c_str()));
        _renameDialog->setParent(this);
        _renameDialog->setWindowModality(Qt::WindowModal);
        _renameDialog->show();
		ui->pb_rename->setDisabled(true);
	}
}

// Udisk
// Udisk = 1
// Udisk = 2
// Udisk = 3
// Udisk = 4
void QtMyFile_MVD::slotNewUDiskJoinin(unsigned short UDISKID)
{
    if(1 == UDISKID)
        _udisk1Status = true;
    if(2 == UDISKID)
        _udisk2Status = true;
    if(3 == UDISKID)
        _udisk3Status = true;
    if(4 == UDISKID)
        _udisk4Status = true;

    if(true == _udisk1Status && false == _udisk1Join)
    {
        QString str = QString("ln -s /dev/udisk1 %1%2/")
                .arg(BASE_PATH).arg(USERSPACE);
        system(str.toStdString().c_str());
        system("sync");
        _udisk1Join = true;
        emit signalNewUDiskJoinin(UDISKID);
        initView(_defaultPath);
        ui->pb_export->show();
//        qDebug() << "New udisk" << UDISKID << "join in";
    }

    if(true == _udisk2Status && false == _udisk2Join)
    {
        /* code */
//        qDebug() << "New udisk" << UDISKID << "join in";
    }


    if(true == _udisk3Status && false == _udisk3Join)
    {
        /* code */
//        qDebug() << "New udisk" << UDISKID << "join in";
    }

    if(true == _udisk4Status && false == _udisk4Join)
    {
        /* code */
//        qDebug() << "New udisk" << UDISKID << "join in";
    }
}

void QtMyFile_MVD::slotUdiskExit(unsigned short UDISKID)
{
    if(1 == UDISKID)
        _udisk1Status = false;
    if(2 == UDISKID)
        _udisk2Status = false;
    if(3 == UDISKID)
        _udisk3Status = false;
    if(4 == UDISKID)
        _udisk4Status = false;

    if(false == _udisk1Status && true == _udisk1Join)
    {
        _udisk1Join = false;
        emit signalUdiskExit(UDISKID);
        initView(_defaultPath);
        ui->pb_export->hide();
        QString link = "/mnt/udisk1"; // U盘软连接路径 ...
        unlink(link.toStdString().c_str());
        remove(link.toStdString().c_str());
        system("sync");
//        qDebug() << "UDisk" << UDISKID << "exit.";
    }

    if(false == _udisk2Status && true == _udisk2Join)
    {
        /* code */
//        qDebug() << "UDisk" << UDISKID << "exit.";
    }

    if(false == _udisk3Status && true == _udisk3Join)
    {
        /* code */
//        qDebug() << "UDisk" << UDISKID << "exit.";
    }

    if(false == _udisk4Status && true == _udisk4Join)
    {
        /* code */
//        qDebug() << "UDisk" << UDISKID << "exit.";
    }
}

void QtMyFile_MVD::on_pb_review_clicked()
{
}

void QtMyFile_MVD::on_pb_export_clicked()
{
    _exportDialog->setParent(this);
    _exportDialog->setWindowModality(Qt::WindowModal);
    _exportDialog->show();
}

bool QtMyFile_MVD::checkUSB() const
{
//    return QFile::exists("/dev/usb1");
      return false;
}

int QtMyFile_MVD::Export(QString src,QString dst)
{
    scanExportItem(src);
    if(_srcList.empty())
    {
        system("sync");
        _exportDetail->close();
        return -2;
    }

    int ret = 0;
    QByteArray sourceHash;
    QCryptographicHash hash(QCryptographicHash::Sha1);
    for(int i = 0; i < _srcList.length(); ++ i)
    {
        if(QFile::exists(_srcList[i]))
        {
            QFile srcFile(_srcList[i]);
            if(srcFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered))
            {
                // 此处不可调换 copy会 偏移文件指针 偏移后不会重置
                QByteArray data = srcFile.readAll();
                srcFile.copy(dst+QFileInfo(_srcList[i]).fileName());
                srcFile.flush();
                srcFile.flush();
                srcFile.close();
                ///////////////////////////////////////////////////////

                hash.reset();
                hash.addData(data);
                sourceHash = hash.result().toHex();

                QFile dstFile(dst+QFileInfo(_srcList[i]).fileName());
                if(dstFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered))
                {
                    dstFile.flush();
                    hash.reset();
                    hash.addData(dstFile.readAll());
                    dstFile.close();

                    if(sourceHash == hash.result().toHex())
                        _exportDetail->appen(QFileInfo(_srcList[i]).fileName()+" 导出成功.",1);
                    else
                    {
                        ret = -1;
                        _exportDetail->appen(QFileInfo(_srcList[i]).fileName()+" 导出失败.",-1);
                        // remove breakdown file
                        dstFile.remove();
                    }
                }
                else
                {
                    ret = -1;
                    _exportDetail->appen(QFileInfo(_srcList[i]).fileName()+" 导出失败.",-1);
                    // remove breakdown file
                    dstFile.remove();
                }
            }
        }
    }
    system("sync");
    return ret;
}

void QtMyFile_MVD::cleanDir(const QString & path)
{
    QString cmd = QString ("rm -rf %1*").arg(path);
    system(cmd.toStdString().c_str());
    system("sync");
}

void QtMyFile_MVD::slotExport(unsigned t)
{
    if(!checkUSB()) return;

    _exportDetail->setParent(this);
    _exportDetail->setWindowModality(Qt::WindowModal);

    /* to do */
    /* code : export something... */

    return;
}
// 将勾选项 加入 操作队列
void QtMyFile_MVD::scanCheckedStatus(const QString srcPath)
{
    _srcList.clear();
    QDirIterator it(srcPath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        if(it.fileName() != "." && it.fileName() != ".." && it.fileName() != "/")
        {
            if(_fileSystemModel->index(it.filePath()).data(Qt::CheckStateRole).toInt())
            {
//                qDebug() << it.fileName() << it.filePath() << Qt::CheckState::Checked;
                _srcList.push_back(it.filePath());
            }
        }
    }
}

void QtMyFile_MVD::scanExportItem(const QString srcPath)
{
    _srcList.clear();
    QDirIterator it(srcPath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        if(it.fileName() != "." && it.fileName() != ".." && it.fileName() != "/")
        {
            if(it.fileInfo().completeSuffix() == "png" || it.fileInfo().completeSuffix() == "pdf")
            {
                _srcList.push_back(it.filePath());
            }
        }
    }
}

// 从操作队列中移除 保护资源 并警告
void QtMyFile_MVD::removeProtectSRC()
{
    if(_srcList.empty()) return;
    for(int i = 0; i < _srcList.size(); ++ i)
    {
        /* to do... */
    }
}

const QString QtMyFile_MVD::getSectionPath(const QModelIndex index)
{
	QModelIndex parentIndex = index.parent();
	QString sectionPath = index.data(Qt::DisplayRole).toString();
	while (parentIndex.isValid())
	{
		if (!parentIndex.data(Qt::CheckStateRole).toInt())
			break;
		else
			sectionPath.push_front(parentIndex.data(Qt::DisplayRole).toString() + QString("/"));
		parentIndex = parentIndex.parent();
	}
	return sectionPath;
}

// mkdir
void QtMyFile_MVD::makePath()
{
    if(_dir->exists(_dscPath))
    {
        for(int i = 0; i < _srcList.size(); ++ i)
        {
            QModelIndex index = _fileSystemModel->index(_srcList[i]);
            if(index.isValid())
            {
                if(_fileSystemModel->isDir(index))
                {
                    // 拿到路径 子串
					QString sectionPath = getSectionPath(index);
					QString path = _dscPath + QString("/") + sectionPath;
					_dir->mkpath(path);
					_srcList.removeAt(i);
                    --i;
					//qDebug() << sectionPath << "\n" << path;
                }
            }
            else
            {
                _srcList.removeAt(i);
				--i;
            }
        }
    }
}

// find a valid and near parent QModelindex.
const QModelIndex QtMyFile_MVD::getNearParent(const QString path)
{
	QModelIndex parentIndex = _fileSystemModel->index(path).parent();
    if (parentIndex.isValid())
	{
		while (parentIndex.isValid())
		{
			if (!parentIndex.data(Qt::CheckStateRole).toBool())
			{
				return parentIndex;
			}
			parentIndex = parentIndex.parent();
		}
	}
	qDebug() << "Error! turn to default path.";
	return _fileSystemModel->index(_defaultPath);
}

// 找下一个最小的文件No坑位 [新建文件,新建文件1,新建文件3...新建文件n] ->[0,1,^,3...n] => 2
// @parameter fileNameListNo : const QList<unsigned int>& 待排序文件No列表
// @return : unsigned int 下一个最小的文件No坑位 0 : 表示可以直接创建 "新建文件"
unsigned int QtMyFile_MVD::findNextID(const QList<unsigned long>& fileNameListNo)
{
    if(!fileNameListNo.isEmpty() && fileNameListNo.size() > 1)
    {
        if(fileNameListNo[0]!=0) return 0;

        // 找最小的文件No坑位
        int i = 0;
        unsigned int next;
        for(; i < fileNameListNo.size() - 1; ++i)
        {
            next = fileNameListNo[i+1];
            if(next - fileNameListNo[i] > 1)
                return (fileNameListNo[i] + 1);
        }
        if(fileNameListNo[i] == next) return next+1;
    }
    else if(!fileNameListNo.isEmpty() && fileNameListNo.size() == 1)
    {
        if(fileNameListNo[0] == 0) return 1;
        else if(fileNameListNo[0] > 0) return 0;
    }
    return 0;
}

void QtMyFile_MVD::on_pb_newFolder_clicked()
{
    QFileInfo info(_dscPath);
    if(_dir->exists(info.absoluteFilePath()))
    {
        QDirIterator dirIter(_dscPath,
                             QStringList()<<"NewFolder*",
                             QDir::NoDotAndDotDot | QDir::Dirs | QDir::NoSymLinks,
                             QDirIterator::NoIteratorFlags);

        // 正则表达式过滤文件迭代器遍历上来的文件迭代器对象 QString QFileinfo::fileName();
        // 只要 文件夹名称是 "NewFolder[0-9]+"
        QList<unsigned long> fileNameListNo;
        QRegExp reg("NewFolder[0-9]+");
        while(dirIter.hasNext())
        {
//            qDebug() << dirIter.next();
            if(dirIter.fileInfo().fileName() == "NewFolder")
                fileNameListNo << 0;

            reg.setPatternSyntax(QRegExp::RegExp2);
            if(reg.exactMatch(dirIter.fileInfo().fileName()))
            {
                QString str = dirIter.fileInfo().fileName().remove("NewFolder");
                fileNameListNo << str.toULong();
            }
        }

        std::sort(fileNameListNo.begin(),fileNameListNo.end());

        // 返回 0 则说明 没有 "NewFolder"
        QString name;
        unsigned int fileNo = findNextID(fileNameListNo);
        if(fileNo == 0)
            name = QObject::tr("NewFolder");
        else
            name = QObject::tr("NewFolder%1").arg(fileNo);
        _fileSystemModel->mkdir(ui->tableView->currentIndex(),name);
    }
}

void QtMyFile_MVD::on_pb_home_clicked()
{
    initView("./");
}
