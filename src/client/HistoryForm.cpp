#include "HistoryForm.h"

QList<struct ListEntry> sessionList;
QList<struct ChatEntry> messages;
QString currNickname = "";

HistoryForm::HistoryForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.sessionList, SIGNAL(currentRowChanged(int)), this, SLOT(onSessionSelect(int)));
    connect(ui.searchButton, SIGNAL(clicked()), this, SLOT(searchBtnHandler()));

    if (openDB() < 0) {
        msgBoxCritical("Unable to open the database.");
        return;
    }

	/* Load all lists */
	sessionList = findSession(LIST_ALL, NULL_STR, NULL_STR, NULL_STR, NULL_STR, NULL_STR);
    renderList();
}

HistoryForm::~HistoryForm()
{

}

void HistoryForm::renderList()
{
    ui.sessionList->clear();
    QList<struct ListEntry>::iterator i;
    for (i = sessionList.begin(); i != sessionList.end(); i++) {
        ui.sessionList->addItem(QStringLiteral("[") + i->createdAt + QStringLiteral("] ") + i->host);
    }
}

void HistoryForm::onSessionSelect(int idx)
{
    if (idx < 0) {
        ui.messageDisplay->clear();
        return;
    }
        

    QList<struct ListEntry>::iterator i = sessionList.begin();
    if (sessionList.count() > 0 && idx < sessionList.count()) {
        i += idx;
        messages = getChatData(i->sessionId, ui.contentField->text().toStdString(),
            ui.nicknameField->text().toStdString(), ui.hostField->text().toStdString(),
            ui.dateEditBegin->text().toStdString(), ui.dateEditEnd->text().toStdString());
        currNickname = i->nickname;
        renderMessages();
    }
	
}

void HistoryForm::searchBtnHandler()
{
    // ui.dateTimeStart.dateTime
    
    sessionList = findSession(SEARCH, ui.contentField->text().toStdString(), 
        ui.nicknameField->text().toStdString(), ui.hostField->text().toStdString(),
        ui.dateEditBegin->text().toStdString(), ui.dateEditEnd->text().toStdString());
    renderList();
}

void HistoryForm::renderMessages()
{
	ui.messageDisplay->clear();
	QList<struct ChatEntry>::iterator i;
	for (i = messages.begin(); i != messages.end(); i++) {
        if (ui.messageDisplay->toPlainText().length() == 0) {
            if (currNickname != i->nickname) {
                ui.messageDisplay->insertHtml(QStringLiteral("<p align='left' style='margin:0;'><b>") + i->nickname
                    + QStringLiteral("</b> ") + i->createdAt + QStringLiteral(
                        "</p><p align='left' style='margin:0;'>"));
                ui.messageDisplay->insertPlainText(i->content);
                ui.messageDisplay->insertHtml("</p><p align='left' style='margin:0;'></p>");
            } else {
                ui.messageDisplay->insertHtml(QStringLiteral("<p align='right' style='margin:0;'>") +
                    i->createdAt + QStringLiteral(" <b>") + i->nickname +
                    QStringLiteral("</b></p><p align='right' style='margin:0;'>"));
                ui.messageDisplay->insertPlainText(i->content);
                ui.messageDisplay->insertHtml("</p><p align='right' style='margin:0;'></p>");
            }

        } else {
            if (currNickname != i->nickname) {
                ui.messageDisplay->insertHtml(QStringLiteral("<br><p align='left' style='margin:0;'><b>") + i->nickname
                    + QStringLiteral("</b> ") + i->createdAt + QStringLiteral(
                        "</p><p align='left' style='margin:0;'>"));
                ui.messageDisplay->insertPlainText(i->content);
                ui.messageDisplay->insertHtml("</p><p align='left' style='margin:0;'></p>");
            } else {
                ui.messageDisplay->insertHtml(QStringLiteral("<br><p align='right' style='margin:0;'>") +
                    i->createdAt + QStringLiteral(" <b>") + i->nickname +
                    QStringLiteral("</b></p><p align='right' style='margin:0;'>"));
                ui.messageDisplay->insertPlainText(i->content);
                ui.messageDisplay->insertHtml("</p><p align='right' style='margin:0;'></p>");
            }

        }

	}

    ui.messageDisplay->verticalScrollBar()->setValue(ui.messageDisplay->verticalScrollBar()->maximum());

}

void HistoryForm::closeEvent(QCloseEvent*)
{
    emit ExitWin();
}