#ifndef CHATTEXTEDITOR_H
#define CHATTEXTEDITOR_H

#include "QtCore/QJsonArray"
#include "QtWidgets/QTextEdit"

#include "context/message.h"

class ChatTextEditorPrivate;

class ChatTextEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit ChatTextEditor(QWidget *parent = 0);
    virtual ~ChatTextEditor();

	void clear();
	QJsonArray extractMessage();
	void insertText(const QString& text, const QTextCharFormat txtFmt, 
		QTextCursor& cur = QTextCursor());
	void insertImage(const QTextImageFormat imageFmt, QTextCursor& cur = QTextCursor());
	void setSendHotkey(const QString& hotkeyStr);

	Q_SIGNAL void imageClicked(const QUrl& imageUrl, QPrivateSignal);
	Q_SIGNAL void linkClicked(const QUrl& linkUrl, QPrivateSignal);
	Q_SIGNAL void send(QPrivateSignal);

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;
	void mousePressEvent(QMouseEvent* e) override;

private:
	Q_DECLARE_PRIVATE(ChatTextEditor);
	Q_DISABLE_COPY(ChatTextEditor);
	QScopedPointer<ChatTextEditorPrivate> d_ptr;
};

#endif // CHATTEXTEDITOR_H
