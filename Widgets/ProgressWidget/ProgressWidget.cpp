#include <QFile>
#include <Network/NetworkManager.h>

#include "ProgressWidget.h"
#include "Libs/GZip/GZip.h"
#include "Constants.h"

ProgressWidget::ProgressWidget(const DownloadInfo downloadInfo) noexcept : downloadInfo(downloadInfo)
{
    ui.setupUi(this);
}

void ProgressWidget::onWaitInfo(const QNetworkReply *const pReply) noexcept
{
    QObject::connect(ui.cancelButton, &QPushButton::clicked, pReply, &QNetworkReply::finished);
}

bool ProgressWidget::startDownload(std::unique_ptr<QNetworkReply> pReply) noexcept
{
    ui.label->setText(Constants::DOWNLOAD_LITERAL);

    QObject::connect(pReply.get(), &QNetworkReply::downloadProgress, this, &ProgressWidget::onDownloadProgress);
    QObject::connect(ui.cancelButton, &QPushButton::clicked, pReply.get(), &QNetworkReply::abort);

    NetworkManager::waitNetworkReply(pReply.get());

    if (pReply->error() != QNetworkReply::NoError)
    {
        close();
        return false;
    }

    ui.label->setText(Constants::EXTRACT_LITERAL);

    QFile file(downloadInfo.to);

    if (!file.open(QFile::WriteOnly))
    {
        close();
        return false;
    }

    file.write(GZip::gzUncompress(pReply->readAll()));

    close();

    return true;
}

void ProgressWidget::onDownloadProgress(const qint64 bytesReceived, const qint64 bytesTotal) const noexcept
{
    if (bytesReceived != bytesTotal)
    {
        ui.progressBar->setValue(static_cast<int>(bytesReceived * 100 / downloadInfo.size));
    }
    else
    {
        ui.progressBar->setValue(100);
    }
}
