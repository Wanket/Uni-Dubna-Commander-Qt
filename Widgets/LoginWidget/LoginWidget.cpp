#include <memory>
#include <Constants.h>

#include "LoginWidget.h"
#include "Widgets/MainWindow/MainWindow.h"
#include "Network/NetworkManager.h"

LoginWidget::LoginWidget() noexcept
{
    ui.setupUi(this);
    setupListeners();
}

void LoginWidget::setupListeners() const noexcept
{
    connect(ui.loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClick);
    connect(ui.exitButton, &QPushButton::clicked, this, &LoginWidget::close);
}

void LoginWidget::onLoginClick() noexcept
{
    ui.loginButton->blockSignals(true);

    if (!ui.loginLineEdit->text().isEmpty() && !ui.passwordLineEdit->text().isEmpty())
    {
        auto manager = std::make_unique<NetworkManager>(Login{ ui.loginLineEdit->text(), ui.passwordLineEdit->text() });

        if (manager->isCorrectLogin() == NetworkManager::CorrectLogin::NotCorrect)
        {
            ui.errorLabel->setText(Constants::WRONG_LOGIN);
            return;
        }

        if (manager->isCorrectLogin() == NetworkManager::CorrectLogin::NetworkError)
        {
            ui.errorLabel->setText(Constants::LOGIN_NETWORK_ERROR);
            return;
        }

        const auto mainWindow = new MainWindow(std::move(manager));
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->show();

        close();
    }

    ui.loginButton->blockSignals(false);
}
