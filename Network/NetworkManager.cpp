#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QStringBuilder>

#include "NetworkManager.h"
#include "Libs/tinyxml2/tinyxml2.h"
#include "Widgets/ProgressWidget/ProgressWidget.h"
#include "Constants.h"

const QString NetworkManager::emptyString;

const QByteArray NetworkManager::emptyByteArray;

NetworkManager::NetworkManager(const Login &login) noexcept : login(login)
{}

template <class T>
T NetworkManager::checkObjectOnNull(const T &object)
{
    return object ? object : throw NullPointerException();
}

std::unique_ptr<QNetworkReply> NetworkManager::GetFtpFiles(const QString &path) noexcept
{
    const QString url = Constants::BASE_GET_FTP_FILES_URL % path % Constants::LOGIN_PARAMETER_LITERAL % login.nickName %
                        Constants::PASSWORD_PARAMETER_LITERAL % login.password;

    auto networkReply = post(url);

    waitNetworkReply(networkReply.get());

    return networkReply;
}

std::unique_ptr<QNetworkReply> NetworkManager::post(const QString &url)
{
    QNetworkRequest networkRequest(url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, Constants::TEXT_HTTP);

    return std::unique_ptr<QNetworkReply>(manager.post(networkRequest, emptyByteArray));
}

void NetworkManager::waitNetworkReply(const QNetworkReply *const networkReply) noexcept
{
    QEventLoop loop;
    QObject::connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

NetworkManager::CorrectLogin NetworkManager::isCorrectLogin() noexcept
{
    const auto reply = GetFtpFiles(Constants::ROOT_PATH);

    if (reply->error() != QNetworkReply::NoError)
    {
        return CorrectLogin::NetworkError;
    }

    return QString(reply->readAll()).indexOf(Constants::PERSONAL_LITERAL) != -1 ? CorrectLogin::Correct
                                                                                : CorrectLogin::NotCorrect;
}

std::list<FolderItem> NetworkManager::getItems(const QString path)
{
    std::list<FolderItem> result;

    const auto reply = GetFtpFiles(path);

    if (reply->error() != QNetworkReply::NoError)
    {
        throw std::exception();
    }

    const auto data = reply->readAll().toStdString();

    tinyxml2::XMLDocument document;

    if (const auto error = document.Parse(data.c_str(), data.size()); error == tinyxml2::XMLError::XML_SUCCESS)
    {
        const auto tbody = checkObjectOnNull(
                               checkObjectOnNull(
                                   checkObjectOnNull(document.FirstChildElement(Constants::DIV))
                               ->FirstChildElement(Constants::TABLE))
                           ->FirstChildElement(Constants::TBODY));

        for (auto *tr = tbody->FirstChildElement(Constants::TR); tr != nullptr; tr = tr->NextSiblingElement())
        {
            const auto ftpFileType = checkObjectOnNull(tr->FirstChildElement(Constants::TD));
            const auto ftpFileName = checkObjectOnNull(ftpFileType->NextSiblingElement());
            const auto ftpFileSize = checkObjectOnNull(ftpFileName->NextSiblingElement());
            const auto ftpFileLastChange = checkObjectOnNull(ftpFileSize->NextSiblingElement());

            const auto isFolder = ftpFileType->GetText() == Constants::FOLDER_LITERAL;
            const auto name = ftpFileName->GetText();
            const auto fileSize = ftpFileSize->GetText();
            const auto lastChange = ftpFileLastChange->GetText();

            result.emplace_back(name, fileSize, lastChange, isFolder);
        }
    }

    return result;
}

bool NetworkManager::downloadFile(const DownloadInfo downloadInfo) noexcept
{
    auto progressWidget = new ProgressWidget(downloadInfo);
    progressWidget->setAttribute(Qt::WA_DeleteOnClose);

    auto index = downloadInfo.from.lastIndexOf(Constants::ROOT_PATH);
    const QString url =
            Constants::BASE_DOWNLOAD_FTP_FILE_URL % downloadInfo.from.left(index) % Constants::NAME_PARAMETER_LITERAL %
            downloadInfo.from.mid(index + 1) % Constants::LOGIN_PARAMETER_LITERAL % login.nickName %
            Constants::PASSWORD_PARAMETER_LITERAL % login.password;

    const auto networkReply = post(url);

    progressWidget->onWaitInfo(networkReply.get());
    progressWidget->show();

    waitNetworkReply(networkReply.get());

    if (networkReply->error() != QNetworkReply::NoError)
    {
        progressWidget->close();
        return false;
    }

    auto json = QJsonDocument::fromJson(networkReply->readAll());

    if (json[Constants::ERROR_MESSAGE_LITERAL] != emptyString)
    {
        progressWidget->close();
        return false;
    }

    return progressWidget->startDownload(std::unique_ptr<QNetworkReply>(manager.get(
            QNetworkRequest(Constants::BASE_DOWNLOAD_FILE_URL + json[Constants::FILE_NAME_LITERAL].toString()))));
}
