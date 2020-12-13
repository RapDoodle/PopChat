#pragma once

#include <QtWidgets/QWidget>
#include "ui_ConnectForm.h"
#include "EntranceForm.h"

class ConnectForm : public QWidget
{
    Q_OBJECT

public:
    ConnectForm(QWidget *parent = Q_NULLPTR);

private slots:
    void btnLoginHandler();
    void toHistory();

private:
    Ui::ConnectFormClass ui;
    EntranceForm* ef;
    //���и�����Ƿ�ɹ�����server��function
};
