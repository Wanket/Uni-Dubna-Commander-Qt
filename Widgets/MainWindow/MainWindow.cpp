#include <QFileDialog>
#include <QDesktopServices>
#include <QStringBuilder>

#include "MainWindow.h"
#include "Widgets/TreeWidgetItem.h"
#include "Constants.h"

MainWindow::MainWindow(std::unique_ptr<NetworkManager> &&networkManagerPtr) noexcept :
        networkManagerPtr(std::move(networkManagerPtr))
{
    ui.setupUi(this);

    setupRootFolder();
    setupContextMenu();

    QObject::connect(ui.treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);
}

void MainWindow::setupRootFolder() noexcept
{
    ui.treeWidget->header()->resizeSection(Constants::Name, 500);
    ui.treeWidget->header()->resizeSection(Constants::Date, 200);

    while (true)
    {
        try
        {
            fillItem(ui.treeWidget->topLevelItem(0), networkManagerPtr->getItems(Constants::ROOT_PATH));
            break;
        }
        catch (const std::exception &exception)
        {
            if (auto *const messageBox = generateMessageBox(Constants::FAILED_TO_GET_FILE_LIST_LITERAL,
                                                            Constants::RETRY_LITERAL,
                                                            Constants::CLOSE_APPLICATION_LITERAL);
                    messageBox->exec() != QMessageBox::Ok)
            {
                QApplication::exit(-1);
            }
        }
    }

    QObject::connect(ui.treeWidget, &QTreeWidget::itemExpanded, this, &MainWindow::setupTreeWidgetChildren);
    QObject::connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onDoubleClick);
}

QMessageBox *
MainWindow::generateMessageBox(const QString &text, const QString &okText, const QString &cancelText) noexcept
{
    auto *messageBox = new QMessageBox(this);

    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->setText(text);
    messageBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    messageBox->setButtonText(QMessageBox::Ok, okText);
    messageBox->setButtonText(QMessageBox::Cancel, cancelText);
    messageBox->show();

    return messageBox;
}

void MainWindow::fillItem(QTreeWidgetItem *ParentItem, const std::list<FolderItem> &items) const noexcept
{
    for (const auto &item : items)
    {
        if (item.name != Constants::POINT_LITERAL && item.name != Constants::DOUBLE_POINT_LITERAL)
        {
            const auto child = new TreeWidgetItem(ParentItem);
            child->setText(Constants::Name, item.name.trimmed());
            child->setText(Constants::Size, item.fileSize.trimmed());
            child->setText(Constants::Date, item.lastChange.trimmed());

            if (item.isFolder)
            {
                const auto dummy = new TreeWidgetItem(child);
                dummy->setText(Constants::Name, Constants::POINT_LITERAL);

                child->addChild(dummy);
                child->setText(Constants::FolderOrFile, Constants::FOLDER_LITERAL);
            }
            else
            {
                child->setText(Constants::FolderOrFile, Constants::FILE_LITERAL);
            }

            ParentItem->addChild(child);
        }
    }
}

void MainWindow::setupTreeWidgetChildren(QTreeWidgetItem *item) noexcept
{
    if (const auto child = item->child(0); child->text(Constants::Name) == Constants::POINT_LITERAL)
    {
        child->setText(Constants::Name, Constants::SPACE_LITERAL);

        while (true)
        {
            try
            {
                fillItem(item, networkManagerPtr->getItems(treeItemToFullPath(item)));
                break;
            }
            catch (const std::exception &exception)
            {
                if (auto *const messageBox = generateMessageBox(Constants::FAILED_TO_GET_FILE_LIST_LITERAL,
                                                                Constants::RETRY_LITERAL, Constants::CANCEL_LITERAL);
                        messageBox->exec() != QMessageBox::Ok)
                {
                    return;
                }
            }
        }

        delete child;
    }
}

QString MainWindow::treeItemToFullPath(const QTreeWidgetItem *treeItem) const noexcept
{
    auto fullPath = treeItem->text(Constants::Name);

    while (treeItem->parent() != nullptr)
    {
        fullPath = treeItem->parent()->text(Constants::Name) % Constants::ROOT_PATH % fullPath;
        treeItem = treeItem->parent();
    }

    return fullPath.mid(9); // skip "Uni-Dubna"
}

MainWindow::~MainWindow() noexcept
{
    QApplication::exit(0);
}

void MainWindow::onDoubleClick(const QTreeWidgetItem *const item, const int column) noexcept
{
    if (item->text(Constants::FolderOrFile) == Constants::FILE_LITERAL)
    {
        const DownloadInfo downloadInfo{ treeItemToFullPath(item), temporaryDir.path() + treeItemToFullPath(item),
                                         item->text(Constants::Size).toUInt() };

        const QDir downloadDir(downloadInfo.to.left(downloadInfo.to.lastIndexOf(Constants::ROOT_PATH)));

        while (!(downloadDir.mkpath(downloadDir.path()) && networkManagerPtr->downloadFile(downloadInfo)))
        {
            if (auto *const messageBox = generateMessageBox(Constants::FAILED_DOWNLOAD_FILE,
                                                            Constants::RETRY_LITERAL, Constants::CANCEL_LITERAL);
                    messageBox->exec() != QMessageBox::Ok)
            {
                return;
            }
        }

        QDesktopServices::openUrl(QUrl::fromLocalFile(downloadInfo.to));
    }
}

void MainWindow::onCustomContextMenu(const QPoint &point) noexcept
{
    if (const auto index = ui.treeWidget->indexAt(point); index.isValid())
    {
        if (contextMenuWidgetItem = ui.treeWidget->itemAt(point); contextMenuWidgetItem && contextMenuWidgetItem->text(
                Constants::FolderOrFile) == Constants::FILE_LITERAL)
        {
            contextMenu.exec(ui.treeWidget->viewport()->mapToGlobal(point));
        }
    }
}

void MainWindow::setupContextMenu() noexcept
{
    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    contextMenu.addAction(Constants::OPEN_LITERAL, this, &MainWindow::onOpenClick);
    contextMenu.addAction(Constants::SAVE_AS_LITERAL, this, &MainWindow::onSaveAsClick);
}

void MainWindow::onSaveAsClick() noexcept
{
    if (contextMenuWidgetItem)
    {
        const DownloadInfo downloadInfo{ treeItemToFullPath(contextMenuWidgetItem),
                                         QFileDialog::getSaveFileName(this, Constants::SAVE_AS_LITERAL,
                                                                      QDir::homePath() % Constants::ROOT_PATH %
                                                                      contextMenuWidgetItem->text(Constants::Name)),
                                         contextMenuWidgetItem->text(1).toUInt() };

        const QDir downloadDir(downloadInfo.to.left(downloadInfo.to.lastIndexOf(Constants::ROOT_PATH)));

        while (!(downloadDir.mkpath(downloadDir.path()) && networkManagerPtr->downloadFile(downloadInfo)))
        {
            if (auto *const messageBox = generateMessageBox(Constants::FAILED_DOWNLOAD_FILE,
                                                            Constants::RETRY_LITERAL, Constants::CANCEL_LITERAL);
                    messageBox->exec() != QMessageBox::Ok)
            {
                return;
            }
        }
    }
}

void MainWindow::onOpenClick() noexcept
{
    if (contextMenuWidgetItem)
    {
        onDoubleClick(contextMenuWidgetItem, 0);
    }
}
