#include "chattextbrowser.h"

#include "QtCore/QDateTime"
#include "QtCore/QJsonObject"
#include "QtCore/QUrl"
#include "QtGui/QTextTable"

#include "context/message.h"

class ChatTextBrowserPrivate final
{
public:
	ChatTextBrowserPrivate(ChatTextBrowser* q)
		: q_ptr(q)
	{
	}

	void doInsertText(const QString& text, const QTextCharFormat txtFmt,
		QTextCursor& cur /*= QTextCursor()*/)
	{
		Q_Q(ChatTextBrowser);
		if (cur.isNull())
		{
			cur = q->textCursor();
		}

		cur.setCharFormat(txtFmt);
		cur.insertText(text);
	}

	void doInsertImage(const QTextImageFormat imageFmt, QTextCursor& cur = QTextCursor())
	{
		Q_Q(ChatTextBrowser);
		if (cur.isNull())
		{
			cur = q->textCursor();
		}

		cur.insertImage(imageFmt);
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
		this->doInsertText(senderTip, cur.charFormat(), cur);
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

		for (auto it = messageItem.msgTypes.constBegin(); it != messageItem.msgTypes.constEnd(); ++it)
		{
			const QJsonObject& msgType = it->toObject();
			if (msgType["type"].toString() == "text")
			{
				const QString& text = msgType["content"].toString();
				this->doInsertText(text, cur.charFormat(), cur);
			}
			else if (msgType["type"].toString() == "image")
			{
				const QString& imageFilePath = msgType["filePath"].toString();
				QTextImageFormat imageFormat;
				imageFormat.setWidth(100);
				imageFormat.setHeight(100);
				imageFormat.setName(imageFilePath);
				this->doInsertImage(imageFormat, cur);
			}
		}

		cur = tb->lastCursorPosition();
	}

	void doMousePressEvent(QMouseEvent* e)
	{
		Q_Q(ChatTextBrowser);
		if (e->button() != Qt::LeftButton)
		{
			return;
		}

		QTextCursor cursor = q->cursorForPosition(e->pos());
		QTextCharFormat charFormat = cursor.charFormat();
		if (charFormat.isImageFormat())
		{
			const QUrl& imageUrl = charFormat.toImageFormat().name();
			if (imageUrl.isValid())
			{
				emit q->imageClicked(imageUrl, ChatTextBrowser::QPrivateSignal());
			}
		}
		else if (charFormat.isAnchor())
		{
			const QUrl& linkUrl = charFormat.anchorHref();
			if (linkUrl.isValid())
			{
				emit q->linkClicked(linkUrl, ChatTextBrowser::QPrivateSignal());
			}
		}
	}

private:
	Q_DECLARE_PUBLIC(ChatTextBrowser);
	ChatTextBrowser* q_ptr;
};

ChatTextBrowser::ChatTextBrowser(QWidget *parent) 
	: QTextBrowser(parent)
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

void ChatTextBrowser::mousePressEvent(QMouseEvent* e)
{
	Q_D(ChatTextBrowser);
	d->doMousePressEvent(e);

	QTextBrowser::mousePressEvent(e);
}
