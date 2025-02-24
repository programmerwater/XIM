#include "mainwgt.h"

#include "QtCore/QDateTime"
#include "QtWidgets/QVBoxLayout"

#include "chattextbrowser.h"
#include "chattexteditor.h"
#include "context/message.h"

class MainWgtPrivate final
{
public:
	MainWgtPrivate(MainWgt* q)
		: q_ptr(q)
	{
	}

	void init()
	{
		Q_Q(MainWgt);
		QVBoxLayout* layout = new QVBoxLayout(q);
		textBrowser_ = new ChatTextBrowser(q);
		layout->addWidget(textBrowser_);
		textEditor_ = new ChatTextEditor(q);
		textEditor_->setFixedHeight(250);
		layout->addWidget(textEditor_);

		QObject::connect(textEditor_, &ChatTextEditor::send, q, &MainWgt::onSend);
		textEditor_->setSendHotkey("Ctrl+Enter");
	}

	void doSend()
	{
		MessageItem item;
		item.time = QDateTime::currentMSecsSinceEpoch();
		static bool self = true;
		item.isSelf = self;
		self = !self;
		item.senderName = "sender";
		item.receiverName = "receiver";
		item.messageFormat = textEditor_->extractMessage();
		textBrowser_->addMessageItem(item);
		textEditor_->clear();
	}

private:
	Q_DECLARE_PUBLIC(MainWgt);

	MainWgt* q_ptr;
	ChatTextBrowser* textBrowser_;
	ChatTextEditor* textEditor_;
};

MainWgt::MainWgt(QWidget* parent)
	: QWidget(parent)
	, d_ptr(new MainWgtPrivate(this))
{
	Q_D(MainWgt);
	d->init();
}

MainWgt::~MainWgt()
{
}

void MainWgt::onSend()
{
	Q_D(MainWgt);
	d->doSend();
}