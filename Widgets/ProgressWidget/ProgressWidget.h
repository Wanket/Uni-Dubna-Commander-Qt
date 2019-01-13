#pragma once

#include <QNetworkReply>

#include "ui_ProgressWidget.h"
#include "Network/DownloadInfo.h"

class ProgressWidget : public QWidget
{
private:
    Ui::ProgressWidget ui;

    DownloadInfo downloadInfo;

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) const noexcept;

public:
    explicit ProgressWidget(DownloadInfo downloadInfo) noexcept;

    void onWaitInfo(const QNetworkReply *const pReply) noexcept;

    bool startDownload(std::unique_ptr<QNetworkReply> pReply) noexcept;
};
