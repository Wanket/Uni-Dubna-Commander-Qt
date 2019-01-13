#pragma once

#include <memory>

#include <QTemporaryDir>
#include <QMenu>
#include <QMessageBox>

#include "ui_MainWindow.h"
#include "Network/NetworkManager.h"

class MainWindow final : public QWidget
{
public:
    explicit MainWindow(std::unique_ptr<NetworkManager> &&networkManagerPtr) noexcept;

    ~MainWindow() noexcept override;

private:
    Ui::MainWindow ui;

    const std::unique_ptr<NetworkManager> networkManagerPtr;

    const QTemporaryDir temporaryDir;

    const QTreeWidgetItem *contextMenuWidgetItem = nullptr;

    QMenu contextMenu;

private:
    inline void setupRootFolder() noexcept;

    void setupTreeWidgetChildren(QTreeWidgetItem *item) noexcept;

    void onDoubleClick(const QTreeWidgetItem *const item, int column) noexcept;

    void onOpenClick() noexcept;

    void fillItem(QTreeWidgetItem *ParentItem, const std::list<FolderItem> &items) const noexcept;

    inline void setupContextMenu() noexcept;

    void onCustomContextMenu(const QPoint &point) noexcept;

    void onSaveAsClick() noexcept;

    QString treeItemToFullPath(const QTreeWidgetItem *treeItem) const noexcept;

    QMessageBox *generateMessageBox(const QString &text, const QString &okText, const QString &cancelText) noexcept;
};
