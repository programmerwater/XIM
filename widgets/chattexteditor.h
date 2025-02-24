#ifndef CHATTEXTEDITOR_H
#define CHATTEXTEDITOR_H

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
	MessageFormat extractMessage() const;
	void insertText(const QString& text, const QTextCharFormat txtFmt, 
		QTextCursor& cur = QTextCursor(), bool replace = false);
	void setSendHotkey(const QString& hotkeyStr);

	Q_SIGNAL void send(QPrivateSignal);

protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	Q_DECLARE_PRIVATE(ChatTextEditor);
	Q_DISABLE_COPY(ChatTextEditor);
	QScopedPointer<ChatTextEditorPrivate> d_ptr;
};

#endif // CHATTEXTEDITOR_H
