#include "anythingitemmodel.h"
#include <QLabel>
#include <QtDebug>

AnythingItemModel::AnythingItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant AnythingItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex AnythingItemModel::index(int row, int column, const QModelIndex &parent) const
{
     return createIndex(row, column);
}

QModelIndex AnythingItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

bool AnythingItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);

    endInsertRows();
    return true;
}

bool AnythingItemModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(QModelIndex(), column, column + count - 1);

    endInsertRows();
    return true;
}

bool AnythingItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    endRemoveRows();
    return true;
}

bool AnythingItemModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(QModelIndex(), column, column + count - 1);

    endRemoveColumns();
    return true;
}

int AnythingItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.row() == -1)
    {
        if (mShowAnything) {
            if (mAnything != nullptr) {
                return int(mAnything->allFiles().size());
            }
            return 0;
        } else {
            if (mMatchResult != nullptr)
                return int(mMatchResult->size());
            return 0;
        }
    }
    return 0;
}

int AnythingItemModel::columnCount(const QModelIndex &parent) const
{
   return 1;
}

QVariant AnythingItemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    else if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            if (mShowAnything) {
                return QString(mAnything->allFiles()[uint64_t(index.row())].c_str());
            } else {
                return QString((*mMatchResult)[uint64_t(index.row())].fullPath.c_str());
            }
        }
        else
            return "---";
    } else
        return QVariant();
}
