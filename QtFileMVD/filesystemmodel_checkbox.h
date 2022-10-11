#ifndef FILESYSTEMMODEL_CHECKBOX_H
#define FILESYSTEMMODEL_CHECKBOX_H

#include <QObject>
#include <QFileSystemModel>
#include <QDebug>
#include <QDate>

class FileSystemModel_checkbox : public QFileSystemModel
{
    Q_OBJECT
public:
    FileSystemModel_checkbox(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
private:
    QMap<QModelIndex, Qt::CheckState> m_checks;
};

#endif // FILESYSTEMMODEL_CHECKBOX_H
