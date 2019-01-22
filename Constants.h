#pragma once

#include <QString>

struct Constants final
{
    static const QString ROOT_PATH;

    static const QString POINT_LITERAL;

    static const QString DOUBLE_POINT_LITERAL;

    static const QString FOLDER_LITERAL;

    static const QString FILE_LITERAL;

    static const QString SPACE_LITERAL;

    static const QString OPEN_LITERAL;

    static const QString SAVE_AS_LITERAL;

    static const QString WRONG_LOGIN;

    static const QString YEAR_FORMAT;

    static const QString TIME_FORMAT;

    static const QString DOWNLOAD_LITERAL;

    static const QString EXTRACT_LITERAL;

    static const QString PERSONAL_LITERAL;

    static const QString LOGIN_NETWORK_ERROR;

    static const QString BASE_GET_FTP_FILES_URL;

    static const QString BASE_DOWNLOAD_FTP_FILE_URL;

    static const QString BASE_DOWNLOAD_FILE_URL;

    static const QString LOGIN_PARAMETER_LITERAL;

    static const QString PASSWORD_PARAMETER_LITERAL;

    static const QString NAME_PARAMETER_LITERAL;

    static const QString TEXT_HTTP;

    static const QString ERROR_MESSAGE_LITERAL;

    static const QString FILE_NAME_LITERAL;

    static const QString RETRY_LITERAL;

    static const QString CLOSE_APPLICATION_LITERAL;

    static const QString CANCEL_LITERAL;

    static const QString FAILED_TO_GET_FILE_LIST_LITERAL;

    static const QString FAILED_DOWNLOAD_FILE;

    static const QString COLON;

    static constexpr const char *DIV = "div";

    static constexpr const char *TABLE = "table";

    static constexpr const char *TBODY = "tbody";

    static constexpr const char *TR = "tr";

    static constexpr const char *TD = "td";

    enum Sections
    {
        Name = 0,
        Size,
        Date,
        FolderOrFile
    };
};
