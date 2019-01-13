#include "FolderItem.h"

FolderItem::FolderItem(const QString &name, const QString &fileSize, const QString &lastChange, bool isFolder) noexcept
        : name(name.trimmed()), fileSize(fileSize.trimmed()), lastChange(lastChange.trimmed()), isFolder(isFolder)
{}
