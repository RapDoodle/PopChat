#pragma once

#include <QtWidgets/QWidget>
#include <QMessageBox>

#include "ui_ConnectForm.h"
#include "EntranceForm.h"
#include "HistoryForm.h"
#include "MessageBox.h"
#include "ChatSocket.h"

class ConnectForm : public QWidget
{
    Q_OBJECT

public:
    ConnectForm(QWidget *parent = Q_NULLPTR);
    EntranceForm* ef;
    HistoryForm* hf;

private slots:
    void btnLoginHandler();
    void toHistory();

private:
    Ui::ConnectFormClass ui;
};
