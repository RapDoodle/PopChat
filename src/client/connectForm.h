#pragma once

#include <QtWidgets/QWidget>
#include "ui_connectForm.h"
#include "entranceForm.h"

class connectForm : public QWidget
{
    Q_OBJECT

public:
    connectForm(QWidget *parent = Q_NULLPTR);

private slots:
    void btnLoginHandler();
    void toHistory();

private:
    Ui::connectFormClass ui;
    entranceForm* ef;
    //还有个检查是否成功连接server的function
};
