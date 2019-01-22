#include <memory>
#include <Constants.h>

#include "LoginWidget.h"
#include "Widgets/MainWindow/MainWindow.h"
#include "Network/NetworkManager.h"

LoginWidget::LoginWidget() noexcept
{
    ui.setupUi(this);
    ui.errorLabel->hide();

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

    if (!(ui.loginLineEdit->text().isEmpty() || ui.passwordLineEdit->text().isEmpty()))
    {
        ui.errorLabel->hide();

        switch (auto manager = std::make_unique<NetworkManager>(
                    Login{ ui.loginLineEdit->text(), ui.passwordLineEdit->text() }); manager->isCorrectLogin())
        {
            case NetworkManager::CorrectLogin::Correct:
            {
                const auto mainWindow = new MainWindow(std::move(manager));
                mainWindow->setAttribute(Qt::WA_DeleteOnClose);
                mainWindow->show();

                close();
                break;
            }
            case NetworkManager::CorrectLogin::NotCorrect:
                ui.errorLabel->setText(Constants::WRONG_LOGIN);
                break;
            case NetworkManager::CorrectLogin::NetworkError:
                ui.errorLabel->setText(Constants::LOGIN_NETWORK_ERROR);
        }

        ui.errorLabel->show();
    }

    ui.loginButton->blockSignals(false);
}
