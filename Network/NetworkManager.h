#pragma once

#include <memory>

#include <QNetworkAccessManager>
#include <Widgets/ProgressWidget/ProgressWidget.h>

#include "FolderItem.h"
#include "Login.h"
#include "DownloadInfo.h"

class NullPointerException final : public std::exception
{};

class NetworkManager final
{
public:
    enum class CorrectLogin
    {
        Correct,
        NotCorrect,
        NetworkError
    };

    static void waitNetworkReply(const QNetworkReply *const networkReply) noexcept;

    explicit NetworkManager(const Login &login) noexcept;

    ProgressWidget::DownloadResult downloadFile(const DownloadInfo downloadInfo) noexcept;

    std::unique_ptr<QNetworkReply> GetFtpFiles(const QString &path) noexcept;

    std::list<FolderItem> getItems(const QString path);

    CorrectLogin isCorrectLogin() noexcept;

private:
    static const QString emptyString;

    static const QByteArray emptyByteArray;

    QNetworkAccessManager manager;

    Login login;

    template <class T>
    inline T checkObjectOnNull(const T &object);

    std::unique_ptr<QNetworkReply> post(const QString &url);
};
