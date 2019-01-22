#include <QFile>
#include <Network/NetworkManager.h>

#include "ProgressWidget.h"
#include "Libs/GZip/GZip.h"
#include "Constants.h"

ProgressWidget::ProgressWidget(const DownloadInfo downloadInfo) noexcept : downloadInfo(downloadInfo)
{
    ui.setupUi(this);
    ui.progressBar->hide();
}

void ProgressWidget::onWaitInfo(const QNetworkReply *const pReply) noexcept
{
    QObject::connect(ui.cancelButton, &QPushButton::clicked, pReply, &QNetworkReply::abort);
}

ProgressWidget::DownloadResult ProgressWidget::startDownload(std::unique_ptr<QNetworkReply> pReply) noexcept
{
    ui.label->setText(Constants::DOWNLOAD_LITERAL);
    ui.progressBar->show();

    QObject::connect(pReply.get(), &QNetworkReply::downloadProgress, this, &ProgressWidget::onDownloadProgress);
    QObject::connect(ui.cancelButton, &QPushButton::clicked, pReply.get(), &QNetworkReply::abort);

    NetworkManager::waitNetworkReply(pReply.get());

    if (const auto error = pReply->error(); error != QNetworkReply::NoError)
    {
        close();

        return error == QNetworkReply::OperationCanceledError ? DownloadResult::Canceled : DownloadResult::Error;

    }

    ui.label->setText(Constants::EXTRACT_LITERAL);

    QFile file(downloadInfo.to);

    if (!file.open(QFile::WriteOnly))
    {
        close();
        return DownloadResult::Error;
    }

    file.write(GZip::gzUncompress(pReply->readAll()));

    close();

    return DownloadResult::Success;
}

void ProgressWidget::onDownloadProgress(const qint64 bytesReceived, const qint64 bytesTotal) const noexcept
{
    ui.progressBar->setValue(
            bytesReceived != bytesTotal ? static_cast<int>(bytesReceived * 100 / downloadInfo.size) : 100);
}
