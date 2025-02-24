#include "chattextbrowser.h"

#include "QtCore/QDateTime"
#include "QtGui/QTextTable"

#include "context/message.h"

class ChatTextBrowserPrivate final
{
public:
	ChatTextBrowserPrivate(ChatTextBrowser* q)
		: q_ptr(q)
	{
	}

	void doAddMessageItem(const MessageItem& messageItem)
	{
		Q_Q(ChatTextBrowser);
		QTextTableFormat tbFmt;
		tbFmt.setMargin(0);
		tbFmt.setBorder(0);
		QVector<QTextLength> constraints;
		constraints << QTextLength(QTextLength::PercentageLength, 100);
		tbFmt.setColumnWidthConstraints(constraints);
		QTextCursor cur = q->textCursor();
		QTextTable* tb = cur.insertTable(2, 1, tbFmt);
		QTextBlockFormat blockFmt;
		if (messageItem.isSelf)
		{
			blockFmt.setAlignment(Qt::AlignRight);
		}
		else
		{
			blockFmt.setAlignment(Qt::AlignLeft);
		}
		cur.setBlockFormat(blockFmt);
		const QString& senderTip = messageItem.senderName +
			QDateTime::fromMSecsSinceEpoch(messageItem.time).toString(" HH:mm:ss");
		q->insertText(senderTip, cur.charFormat(), cur);
		cur.movePosition(QTextCursor::NextCell);

		QTextTable* innerTb;
		QTextTableFormat innertbFmt;
		innertbFmt.setMargin(0);
		innertbFmt.setBorder(0);
		if (messageItem.isSelf)
		{
			innertbFmt.setAlignment(Qt::AlignRight);
		}
		else
		{
			innertbFmt.setAlignment(Qt::AlignLeft);
		}
		QVector<QTextLength> innerConstraints;
		innerConstraints << QTextLength(QTextLength::VariableLength, 0);
		innertbFmt.setColumnWidthConstraints(innerConstraints);
		innerTb = cur.insertTable(1, 1, innertbFmt);
		q->insertText(messageItem.messageFormat.content, cur.charFormat(),
			innerTb->firstCursorPosition());
		cur = tb->lastCursorPosition();
	}

private:
	Q_DECLARE_PUBLIC(ChatTextBrowser);
	ChatTextBrowser* q_ptr;
};

ChatTextBrowser::ChatTextBrowser(QWidget *parent) 
	: ChatTextEditor(parent)
	, d_ptr(new ChatTextBrowserPrivate(this))
{
	setReadOnly(true);
}


ChatTextBrowser::~ChatTextBrowser()
{
}

void ChatTextBrowser::addMessageItem(const MessageItem& messageItem) 
{
	Q_D(ChatTextBrowser);
	d->doAddMessageItem(messageItem);
}
