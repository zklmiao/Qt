#include "filesystemmodel_checkbox.h"

FileSystemModel_checkbox::FileSystemModel_checkbox(QObject *parent)
    : QFileSystemModel(parent)
{
}

Qt::ItemFlags FileSystemModel_checkbox::flags(const QModelIndex &index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant FileSystemModel_checkbox::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.isValid() && 3 == index.column())
    {
        if(role == Qt::ItemDataRole::DisplayRole)
        {
            QVariant value = lastModified(index).toString("yyyy/MM/dd hh:mm:ss");
            return value;
        }
    }

    if(Qt::TextAlignmentRole == role)
    {
        QVariant value = QFileSystemModel::data(index, role);
        if(value.isValid())
        {
            value = (int)(Qt::AlignLeft | Qt::AlignVCenter);
        }
        return value;
    }

    if(0 == index.column() && role == Qt::CheckStateRole)
    {
        if(m_checks.contains(index))
            return m_checks[index] ? Qt::Checked : Qt::Unchecked;
        else
        {
            int iChecked = Qt::Unchecked;
            QModelIndex parentIndex = index.parent();
            while (parentIndex.isValid())
            {
                if(m_checks[parentIndex])
                {
                    iChecked = Qt::Checked;
                    break;
                }
                else
                {
                    parentIndex = parentIndex.parent();
                }
            }
//            if(iChecked == Qt::CheckState::Checked)
//            {
//                m_checks[index] = Qt::CheckState::Checked;
//            }
//            else
//            {
//                m_checks[index] = Qt::CheckState::Unchecked;
//            }
            return iChecked;
        }
    }

    if(role != Qt::DisplayRole)
        return QFileSystemModel::data(index, role);

    return QFileSystemModel::data(index, role);
}

bool FileSystemModel_checkbox::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole && index.column() == 0)
    {
        if(value == Qt::Unchecked)
        {
            m_checks[index] = Qt::Unchecked;
            emit dataChanged(index,index);
        }
        else if(value == Qt::Checked)
        {
            m_checks[index] = Qt::Checked;
            emit dataChanged(index,index);
        }

        if(hasChildren(index))
        {
            int iChildCount = rowCount(index);
            if(iChildCount > 0)
            {
                for(int i = 0; i < iChildCount; ++ i)
                {
                    QModelIndex childIndex = index.child(i,0);
                    setData(childIndex,value,Qt::CheckStateRole);
                }
            }
        }
    }
    return QFileSystemModel::setData(index, value, role);
}

QVariant FileSystemModel_checkbox::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Orientation::Horizontal)
        {
            switch (section) {
            case 0:
                return QString("名称");
            break;
            case 1:
                return QString("大小");
            break;
            case 2:
                return QString("类型");
            break;
            case 3:
                return QString("修改日期");
            break;
            }
        }
//        else if(orientation == Qt::Orientation::Vertical)
//        {
//            switch (section) {
//            case 0:
//                return tr("名称");
//            break;
//            case 1:
//                return tr("大小");
//            break;
//            case 2:
//                return tr("类型");
//            break;
//            case 3:
//                return tr("修改日期");
//            break;
//            }
//        }
    }
    return QVariant();
}
