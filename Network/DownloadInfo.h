#pragma once

#include <QString>

struct DownloadInfo final
{
    QString from;
    QString to;
    qint64 size;
};
