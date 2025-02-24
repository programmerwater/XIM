#include "chattexteditor.h"

#include "QtCore/QDebug"
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
		QTextCursor& cur /*= QTextCursor()*/, bool replace /*= false*/)
	{
		Q_Q(ChatTextEditor);
		if (replace)
		{
			q->clear();
		}
		if (cur.isNull())
		{
			cur = q->textCursor();
		}

		cur.setCharFormat(txtFmt);
		cur.insertText(text);
	}

	MessageFormat doExtractMessage() const
	{
		Q_Q(const ChatTextEditor);
		QStringList content;
		int blockCnt = 0;
		QTextBlock block = q->document()->begin();
		while (true)
		{
			if (!block.isValid())
			{
				break;
			}
			if (block != q->document()->begin())
			{
				content << "\n";
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
				qDebug() << "------text fragment " << textFragmentCnt << "begin------";
				QTextFragment currentFragment = it.fragment();
				if (currentFragment.isValid())
				{
					content << currentFragment.text();
					qDebug() << "text: " << currentFragment.text();
				}
				qDebug() << "------text fragment " << textFragmentCnt << "end------";
				++it;
			}
			qDebug() << "------block " << blockCnt << "end------\n";
			blockCnt++;
			block = block.next();
		}
		MessageFormat item;
		item.content = content.join("");

		return item;
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

private:
	Q_DECLARE_PUBLIC(ChatTextEditor);
	ChatTextEditor* q_ptr;
	QString sendHotkey_;
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
	QTextCursor& cur /*= QTextCursor()*/, bool replace /*= false*/)
{
	Q_D(ChatTextEditor);
	d->doInsertText(text, txtFmt, cur, replace);
}

MessageFormat ChatTextEditor::extractMessage() const
{
	Q_D(const ChatTextEditor);
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
