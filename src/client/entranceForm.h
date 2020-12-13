#pragma once

#include <QWidget>
#include "ui_entranceForm.h"
#include "chatForm.h"

class entranceForm : public QWidget
{
	Q_OBJECT

public:
	entranceForm(QWidget *parent = Q_NULLPTR);
	~entranceForm();

private slots:
	void checkCreate();
	void checkJoin();

private:
	Ui::entranceFormClass ui;
	chatForm* room;
	void tochatForm(int roomID);
};
