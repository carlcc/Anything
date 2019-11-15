#pragma once

#include <QAbstractItemModel>
#include <anything/Anything.h>
#include <memory>
using MatchResultVector = at::ATVector<at::MatchResult, 4096>;

class AnythingItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit AnythingItemModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setAnything(std::shared_ptr<at::Anything> anything) {
        mAnything = anything;
    }

    void setMatchResult(std::shared_ptr<MatchResultVector> result) {
        mMatchResult = result;
    }

    void setShowAnything(bool b) {
        mShowAnything = b;
    }

private:
    std::shared_ptr<at::Anything> mAnything {nullptr};
    std::shared_ptr<MatchResultVector> mMatchResult;
    bool mShowAnything {false}; // show anything of match result
};

