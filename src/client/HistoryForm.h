#pragma once

#include <QWidget>
#include <QtSql/QSqlQuery>
#include <QScrollBar>
#include "ui_HistoryForm.h"
#include "MessageBox.h"

#include "db.h"

class HistoryForm : public QWidget
{
	Q_OBJECT

public:
	HistoryForm(QWidget *parent = Q_NULLPTR);
	~HistoryForm();

private:
	Ui::HistoryForm ui;
	void renderList();
	void renderMessages();

private slots:
	void onSessionSelect(int idx);
	void searchBtnHandler();
};
