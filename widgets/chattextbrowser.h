#ifndef ChatTextBrowser_H
#define ChatTextBrowser_H

#include "chattexteditor.h"
#include "context/message.h"

class ChatTextBrowserPrivate;

class ChatTextBrowser  final : public ChatTextEditor
{
    Q_OBJECT

public:
	explicit ChatTextBrowser(QWidget *parent = 0);
	~ChatTextBrowser();

    void addMessageItem(const MessageItem& messageItem);

private:
	Q_DECLARE_PRIVATE(ChatTextBrowser);
	Q_DISABLE_COPY(ChatTextBrowser);

	QScopedPointer<ChatTextBrowserPrivate> d_ptr;
};

#endif // ChatTextBrowser_H
