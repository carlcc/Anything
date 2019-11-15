#pragma once

#include <QMainWindow>
#include <thread>
#include <memory>
#include <anything/AnythingConfig.h>
#include "simplethreadsafequeue.h"
#include "anythingitemmodel.h"
#include <anything/Anything.h>

class QTextEdit;
class QListView;
class QTableView;
class QLabel;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void resizeEvent(QResizeEvent* event) override;

Q_SIGNALS:
    void sigDataBaseUpdated(std::shared_ptr<at::Anything> anything);
    void sigMatchResult();

private slots:
    void onDataBaseUpdated(std::shared_ptr<at::Anything> anything);
    void onMatchResult();

    void onTextChanged();

private:
    using Task = std::function<void()>;

    QTextEdit* mTextEdit;
    QTableView* mTableView;
    QLabel* mStatusLabel;
    AnythingItemModel* mTableModel;

    std::shared_ptr<at::Anything> mAnything;
    std::shared_ptr<MatchResultVector> mMatchResult;
    std::shared_ptr<MatchResultVector> mMatchResultBack;
    SimpleThreadSafeQueue<Task> mTaskQueue;
    std::thread mWorkingThread;
    std::thread mReloadingThread;
    at::AnythingConfig mConfig;
    bool mStopThread;
    bool mStopTask;
};
