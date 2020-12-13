#pragma once

#include <QtWidgets/QWidget>
#include "ui_hostConnsct.h"
#include "roomEnter.h"

class hostConnsct : public QWidget
{
    Q_OBJECT

public:
    hostConnsct(QWidget *parent = Q_NULLPTR);

private slots:
    void toRoomEnter();
    void toHistory();

private:
    Ui::hostConnsctClass ui;
    roomEnter* re;
    //���и�����Ƿ�ɹ�����server��function
};
