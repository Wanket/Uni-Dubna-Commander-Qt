#pragma once

#include <QString>

struct FolderItem final
{
    QString name;
    QString fileSize;
    QString lastChange;
    bool isFolder;

    FolderItem(const QString &name, const QString &fileSize, const QString &lastChange, bool isFolder) noexcept;
};
