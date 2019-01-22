#pragma once

#include <QNetworkReply>

#include "ui_ProgressWidget.h"
#include "Network/DownloadInfo.h"

class ProgressWidget final : public QWidget
{
public:
    enum class DownloadResult
    {
        Success,
        Error,
        Canceled
    };
    
    explicit ProgressWidget(DownloadInfo downloadInfo) noexcept;

    void onWaitInfo(const QNetworkReply *const pReply) noexcept;

    DownloadResult startDownload(std::unique_ptr<QNetworkReply> pReply) noexcept;

private:
    Ui::ProgressWidget ui;

    DownloadInfo downloadInfo;

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) const noexcept;
};
