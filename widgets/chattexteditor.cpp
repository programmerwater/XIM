#include "chattexteditor.h"

#include "QtCore/QDebug"
#include "QtCore/QJsonArray"
#include "QtCore/QJsonDocument"
#include "QtCore/QJsonObject"
#include "QtGui/QTextBlock"

class ChatTextEditorPrivate final
{
public:
	ChatTextEditorPrivate(ChatTextEditor* q)
		: q_ptr(q)
	{
	}

	~ChatTextEditorPrivate()
	{
	}

	void doInsertText(const QString& text, const QTextCharFormat txtFmt,
		QTextCursor& cur /*= QTextCursor()*/)
	{
		Q_Q(ChatTextEditor);
		if (cur.isNull())
		{
			cur = q->textCursor();
		}

		cur.setCharFormat(txtFmt);
		QTextCharFormat originalFmt = cur.charFormat();
		cur.insertText(text);
		cur.setCharFormat(originalFmt);
	}

	void doInsertImage(const QTextImageFormat imageFmt, QTextCursor& cur = QTextCursor())
	{
		Q_Q(ChatTextEditor);
		if (cur.isNull())
		{
			cur = q->textCursor();
		}

		cur.insertImage(imageFmt);
	}

	QJsonArray doExtractMessage()
	{
		Q_Q(ChatTextEditor);
		int blockCnt = 0;
		QTextBlock block = q->document()->begin();
		QJsonArray msgContent;
		while (true)
		{
			if (!block.isValid())
			{
				break;
			}
			if (block != q->document()->begin())
			{
				content_ << "\n";
			}
			qDebug() << "------block " << blockCnt << "begin------";
			int textFragmentCnt = 0;
			QTextBlock::iterator it = block.begin();
			while(true)
			{
				if (it.atEnd())
				{
					break;
				}
				textFragmentCnt++;
				QTextFragment currentFragment = it.fragment();
				if(!currentFragment.isValid())
				{
					++it;
					continue;
				}
				
				QTextImageFormat imageFmt = currentFragment.charFormat().toImageFormat();
				if (imageFmt.isValid() && !imageFmt.isEmpty()) 
				{
					const QJsonObject& currentMsgType = generanteCurrentMsgType();
					if (currentType_ != ContentType::Image && !currentMsgType.isEmpty())
					{
						msgContent << currentMsgType;
					}

					currentType_ = ContentType::Image;
					content_ << imageFmt.name();

					qDebug() << "------image fragment " << textFragmentCnt << "end------";
					qDebug() << "image: " << imageFmt.name();
					qDebug() << "------image fragment " << textFragmentCnt << "end------";
				}
				else
				{
					const QJsonObject& currentMsgType = generanteCurrentMsgType();
					if (currentType_ != ContentType::Text && !currentMsgType.isEmpty())
					{
						msgContent << currentMsgType;
					}

					currentType_ = ContentType::Text;
					content_ << currentFragment.text();

					qDebug() << "------text fragment " << textFragmentCnt << "begin------";
					qDebug() << "text: " << currentFragment.text();
					qDebug() << "------text fragment " << textFragmentCnt << "end------";
				}
				++it;
			}
			qDebug() << "------block " << blockCnt << "end------\n";
			blockCnt++;
			block = block.next();
		}
		const QJsonObject& currentMsgType = generanteCurrentMsgType();
		if (!currentMsgType.isEmpty())
		{
			msgContent << currentMsgType;
		}
		QJsonDocument jsonDoc(msgContent);
		qDebug() << "msgContent: " << jsonDoc.toJson(QJsonDocument::Indented);
	
		return msgContent;
	}

	void doClear()
	{
		Q_Q(ChatTextEditor);
		if (q->document())
		{
			q->document()->clear();
		}
	}

	void doSetSendHotkey(const QString& hotkeyStr)
	{
		if (sendHotkey_ != hotkeyStr)
		{
			sendHotkey_ = hotkeyStr;
		}
	}

	bool doEventFilter(QObject* watched, QEvent* event)
	{
		static QList<int> queue_key_enter{ {Qt::Key_Enter } };
		static QList<int> queue_key_ctrl_enter{ {Qt::Key_Control},{Qt::Key_Enter} };
		static QList<int> queue_key;

		Q_Q(ChatTextEditor);
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		int key = keyEvent->key();
		if (key == Qt::Key_Return)
		{
			key = Qt::Key_Enter;
		}
		if (key == Qt::Key_Enter || key == Qt::Key_Control)
		{
			queue_key.append(key);
		}
		else
		{
			queue_key.clear();
		}

		if (queue_key.mid(queue_key.size() - 2) == queue_key_ctrl_enter)
		{
			if (sendHotkey_ == QString("Ctrl+Enter"))
			{
				queue_key.clear();
				emit q->send(ChatTextEditor::QPrivateSignal());
			}
			else
			{
				q->textCursor().insertBlock();
			}
			queue_key.clear();
			return true;
		}
		else if (queue_key.mid(queue_key.size() - 1) == queue_key_enter)
		{
			if (sendHotkey_ == QString("Enter"))
			{
				queue_key.clear();
				emit q->send(ChatTextEditor::QPrivateSignal());
			}
			else
			{
				q->textCursor().insertBlock();
			}
			queue_key.clear();
			return true;
		}

		if (queue_key.count() >= 2)
		{
			queue_key.clear();
		}
		return false;
	}

	void doMousePressEvent(QMouseEvent* e)
	{
		if (e->button() != Qt::LeftButton)
		{
			return;
		}

		Q_Q(ChatTextEditor);
		QTextCursor cursor = q->cursorForPosition(e->pos());
		QTextCharFormat charFormat = cursor.charFormat();
		if (charFormat.isImageFormat())
		{
			const QUrl& imageUrl = charFormat.toImageFormat().name();
			if (imageUrl.isValid())
			{
				//emit q->imageClicked(imageUrl, ChatTextEditor::QPrivateSignal());
			}
		}
		else if (charFormat.isAnchor())
		{
			const QUrl& linkUrl = charFormat.anchorHref();
			if (linkUrl.isValid())
			{
				emit q->linkClicked(linkUrl, ChatTextEditor::QPrivateSignal());
			}
		}
	}

private:
	enum class ContentType
	{
		Unknown,
		Text,
		Image
	};

	Q_DECLARE_PUBLIC(ChatTextEditor);
	ChatTextEditor* q_ptr;
	ContentType currentType_ = ContentType::Unknown;
	QString sendHotkey_;
	QStringList	content_;

	QJsonObject generanteCurrentMsgType()
	{
		const QString& targetContent = content_.join("");
		if (targetContent.isEmpty())
		{
			return QJsonObject();
		}

		QJsonObject msgType;
		switch (currentType_)
		{
		case ContentType::Text:
			msgType["type"] = "text";
			msgType["content"] = targetContent;
			break;
		case ContentType::Image:
			msgType["type"] = "image";
			msgType["filePath"] = targetContent;
			break;
		default:
			break;
		}
		
		content_.clear();
		currentType_ = ContentType::Unknown;
		return msgType;
	}
};

ChatTextEditor::ChatTextEditor(QWidget *parent)
	: QTextEdit(parent)
	, d_ptr(new ChatTextEditorPrivate(this))
{
	installEventFilter(this);
}


ChatTextEditor::~ChatTextEditor()
{

}

void ChatTextEditor::insertText(const QString& text, const QTextCharFormat txtFmt, 
	QTextCursor& cur /*= QTextCursor()*/)
{
	Q_D(ChatTextEditor);
	d->doInsertText(text, txtFmt, cur);
}

void ChatTextEditor::insertImage(const QTextImageFormat imageFmt, QTextCursor& cur /*= QTextCursor()*/)
{
	Q_D(ChatTextEditor);
	d->doInsertImage(imageFmt, cur);
}

QJsonArray ChatTextEditor::extractMessage()
{
	Q_D(ChatTextEditor);
	return d->doExtractMessage();
}

void ChatTextEditor::clear()
{
	Q_D(ChatTextEditor);
	d->doClear();
	QTextEdit::clear();
}

void ChatTextEditor::setSendHotkey(const QString& hotkeyStr)
{
	Q_D(ChatTextEditor);
	d->doSetSendHotkey(hotkeyStr);
}

bool ChatTextEditor::eventFilter(QObject *watched, QEvent *event)
{
	if (watched != this || event->type() != QEvent::KeyPress)
	{
		return QTextEdit::eventFilter(watched, event);
	}
	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	if (!keyEvent)
	{
		return QTextEdit::eventFilter(watched, event);
	}
	Q_D(ChatTextEditor);
	if (d->doEventFilter(watched, event))
	{
		return true;
	}
	return QTextEdit::eventFilter(watched, event);
}

void ChatTextEditor::mousePressEvent(QMouseEvent* e)
{
	Q_D(ChatTextEditor);
	d->doMousePressEvent(e);
	QTextEdit::mousePressEvent(e);
}
