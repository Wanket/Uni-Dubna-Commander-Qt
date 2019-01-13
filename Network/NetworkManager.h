#pragma once

#include <memory>

#include <QNetworkAccessManager>

#include "FolderItem.h"
#include "Login.h"
#include "DownloadInfo.h"

class NullPointerException : public std::exception
{};

class NetworkManager
{
public:
    enum class CorrectLogin
    {
        Correct,
        NotCorrect,
        NetworkError
    };

    explicit NetworkManager(const Login &login) noexcept;

    CorrectLogin isCorrectLogin() noexcept;

    bool downloadFile(const DownloadInfo downloadInfo) noexcept;

    static void waitNetworkReply(const QNetworkReply *const networkReply) noexcept;

    std::unique_ptr<QNetworkReply> GetFtpFiles(const QString &path) noexcept;

    std::list<FolderItem> getItems(const QString path);

private:
    QNetworkAccessManager manager;

    Login login;

    static const QString emptyString;

    static const QByteArray emptyByteArray;

    template <class T>
    inline T checkObjectOnNull(const T &object);

    std::unique_ptr<QNetworkReply> post(const QString &url);
};
