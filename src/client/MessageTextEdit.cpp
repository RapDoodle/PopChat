#include "MessageTextEdit.h"

MessageTextEdit::MessageTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	ui.setupUi(this);
}

MessageTextEdit::~MessageTextEdit()
{
}

void MessageTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return) {
        emit messageEditEnterKeyPressed();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}