#pragma once

#include <QByteArray>

class GZip
{
public:
    static QByteArray gzUncompress(const QByteArray &data);
};
