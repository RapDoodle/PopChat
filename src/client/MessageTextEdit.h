#pragma once

#include <QPlainTextEdit>
#include "MessageBox.h"
#include "ui_MessageTextEdit.h"

class MessageTextEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	MessageTextEdit(QWidget *parent = Q_NULLPTR);
	~MessageTextEdit();
	void keyPressEvent(QKeyEvent* event);

private:
	Ui::MessageTextEdit ui;

signals:
	void messageEditEnterKeyPressed();
};