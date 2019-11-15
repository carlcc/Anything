#include "mainwindow.h"
#include "anythingitemmodel.h"
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QListView>
#include <QResizeEvent>
#include <QTableView>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <anything/Anything.h>
#include <anything/AnythingConfig.h>
#include <QDebug>
#include <cstdlib>

const int kTextEditHeight = 30;
const int kStatusLabelHeight = 30;
const int kStatusLabelLeftMargin = 4;
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), mTaskQueue(1)
{
    resize(800, 500);
    move(200, 200);

    mTextEdit = new QTextEdit(this);
    mTableView = new QTableView(this);
    mStatusLabel = new QLabel(this);
    mTableView->move(0, kTextEditHeight);

    mTableModel = new AnythingItemModel(mTableView);
    mTableView->setModel(mTableModel);
    mTableView->verticalHeader()->hide();
    mTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    mStatusLabel->setText("Indexing...");
    mStatusLabel->move(kStatusLabelLeftMargin, size().height() - kStatusLabelHeight);
    mStatusLabel->resize(size().width() - kStatusLabelLeftMargin, kStatusLabelHeight);

    mMatchResult = std::make_shared<MatchResultVector>();
    mMatchResultBack = std::make_shared<MatchResultVector>();
    mTextEdit->setDisabled(true);
    mTextEdit->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);

    qRegisterMetaType<std::shared_ptr<at::Anything>>("std::shared_ptr<at::Anything>");
    QObject::connect(this, SIGNAL(sigDataBaseUpdated(std::shared_ptr<at::Anything>)), this, SLOT(onDataBaseUpdated(std::shared_ptr<at::Anything>)));
    QObject::connect(mTextEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    QObject::connect(this, SIGNAL(sigMatchResult()), this, SLOT(onMatchResult()));

    if (!mConfig.load("anything.json")) {
        mConfig.reset();
        mConfig.addSearchPath(getenv("HOME"));
    }
    mAnything = nullptr;
    mStopThread = false;
    mWorkingThread = std::thread([this]() {
        bool ok;
        while (!mStopThread) {
            auto op = mTaskQueue.get(500, &ok);
            if (ok) {
                op();
            }
        }
    });

    mReloadingThread = std::thread([this]() {
        while (!mStopThread) {
            auto loadingAnything = std::make_shared<at::Anything>(mConfig);
            qDebug() << "Updateing...";
            loadingAnything->reload(mStopThread);

            emit sigDataBaseUpdated(loadingAnything);
            loadingAnything = nullptr;
            for (int i = 0; i < 600 * 30; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                if (mStopThread) {
                    break;
                }
            }
        }
    });
}

MainWindow::~MainWindow()
{
    mStopThread = true;
    if (mWorkingThread.joinable()) {
        mWorkingThread.join();
    }
    if (mReloadingThread.joinable()) {
        mReloadingThread.join();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    mTextEdit->resize(event->size().width(), kTextEditHeight);
    mStatusLabel->resize(event->size().width() - kStatusLabelLeftMargin, kStatusLabelHeight);
    mTableView->resize(event->size().width(), event->size().height() - kTextEditHeight - kStatusLabelHeight);
    mStatusLabel->move(kStatusLabelLeftMargin, event->size().height() - kStatusLabelHeight);
}

void MainWindow::onDataBaseUpdated(std::shared_ptr<at::Anything> anything)
{
    if (mAnything == nullptr && !mTextEdit->isEnabled()) {
        mStatusLabel->setText("Done...");
    }

    mAnything = anything;
    if (mTextEdit->toPlainText().isEmpty()) {
        // Only the first time database updated or when nothing is being searched
        // Then update the view
        mTextEdit->setDisabled(false);
        int oldRows = mTableModel->rowCount();
        int newRows = int(mAnything->allFiles().size());
        mTableModel->setAnything(mAnything);
        mTableModel->setShowAnything(mTextEdit->toPlainText().isEmpty());
        if (newRows > oldRows) {
            mTableModel->insertRows(0, newRows - oldRows);
        } else if (newRows < oldRows) {
            mTableModel->removeRows(0, oldRows - newRows);
        }
        mStatusLabel->setText(QString("Done... %1 files").arg(newRows));
        mTableView->update();
    }
}


void MainWindow::onMatchResult()
{
    std::swap(mMatchResult, mMatchResultBack);

    int oldRows = mTableModel->rowCount();
    int newRows = int(mMatchResult->size());
    mTableModel->setShowAnything(false);
    mTableModel->setMatchResult(mMatchResult);
    if (newRows > oldRows) {
        mTableModel->insertRows(0, newRows - oldRows);
    } else if (newRows < oldRows) {
        mTableModel->removeRows(0, oldRows - newRows);
    }
    mStatusLabel->setText(QString("Done... %1 files").arg(newRows));
    mTableView->update();
}

void MainWindow::onTextChanged()
{
    if (mAnything == nullptr) {
        return;
    }

    auto str = mTextEdit->toPlainText().toStdString();
    mStopTask = true;

    if (str.empty()) {
        onDataBaseUpdated(mAnything);
    } else {
        mStatusLabel->setText("Searching...");
        mTaskQueue.put([this, str]() {
            mStopTask = false;
            auto anything = mAnything;
            anything->matchRegex(*mMatchResultBack, str, mStopTask);
            if (!mStopTask) {
                emit sigMatchResult();
            }
        });
    }
}
