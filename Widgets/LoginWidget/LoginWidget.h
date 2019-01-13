#pragma once

#include "ui_LoginWidget.h"

class LoginWidget : public QWidget
{
public:
    LoginWidget() noexcept;

    inline void setupListeners() const noexcept;

    void onLoginClick() noexcept;

private:
    Ui::LoginWidget ui;
};
