#include <QApplication>

#include "Widgets/LoginWidget/LoginWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto loginWidget = new LoginWidget();
    loginWidget->setAttribute(Qt::WA_DeleteOnClose);
    loginWidget->show();

    return QApplication::exec();
}
