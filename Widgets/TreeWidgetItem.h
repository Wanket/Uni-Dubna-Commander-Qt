#pragma once

#include <QTreeWidgetItem>

class TreeWidgetItem final : public QTreeWidgetItem
{
public:
    explicit TreeWidgetItem(QTreeWidgetItem *pItem) noexcept;

    bool operator<(const QTreeWidgetItem &other) const noexcept override;

private:
    const static QLocale locale;
};
