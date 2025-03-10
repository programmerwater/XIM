#ifndef ChatTextBrowser_H
#define ChatTextBrowser_H

#include "QtWidgets/QTextBrowser"

#include "chattexteditor.h"
#include "context/message.h"

class ChatTextBrowserPrivate;

class ChatTextBrowser  final : public QTextBrowser
{
    Q_OBJECT

public:
	explicit ChatTextBrowser(QWidget *parent = 0);
	~ChatTextBrowser();

    void addMessageItem(const MessageItem& messageItem);

	Q_SIGNAL void imageClicked(const QUrl& imageUrl, QPrivateSignal);
	Q_SIGNAL void linkClicked(const QUrl& linkUrl, QPrivateSignal);

protected:
	void mousePressEvent(QMouseEvent* e) override;

private:
	Q_DECLARE_PRIVATE(ChatTextBrowser);
	Q_DISABLE_COPY(ChatTextBrowser);

	QScopedPointer<ChatTextBrowserPrivate> d_ptr;
};

#endif // ChatTextBrowser_H
