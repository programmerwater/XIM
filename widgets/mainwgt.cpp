#include "mainwgt.h"

#include "QtCore/QDateTime"
#include "QtGui/QDesktopServices"
#include "QtWidgets/QVBoxLayout"

#include "chattextbrowser.h"
#include "chattexteditor.h"
#include "chattexteditortool.h"
#include "context/message.h"

class MainWgtPrivate final : public QObject
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
		ChatTextEditorTool* chatTool = new ChatTextEditorTool(q);
		chatTool->setFixedHeight(25);
		layout->addWidget(chatTool);
		textEditor_ = new ChatTextEditor(q);
		textEditor_->setFixedHeight(250);
		layout->addWidget(textEditor_);

		textEditor_->setSendHotkey("Ctrl+Enter");

		QObject::connect(chatTool, &ChatTextEditorTool::insertImage,
			this, &MainWgtPrivate::onInsertImageFileToEditor);
		QObject::connect(textBrowser_, &ChatTextBrowser::imageClicked,
			this, &MainWgtPrivate::onImageClicked);
		QObject::connect(textEditor_, &ChatTextEditor::imageClicked,
			this, &MainWgtPrivate::onImageClicked);
		QObject::connect(textEditor_, &ChatTextEditor::send, 
			this, &MainWgtPrivate::onSend);
	}

	void onSend()
	{
		if (!textEditor_)
		{
			return;
		}

		MessageItem item;
		item.time = QDateTime::currentMSecsSinceEpoch();
		static bool self = true;
		item.isSelf = self;
		self = !self;
		item.senderName = "sender";
		item.receiverName = "receiver";
		item.msgTypes = textEditor_->extractMessage();
		textBrowser_->addMessageItem(item);
		textEditor_->clear();
	}

	void onInsertImageFileToEditor(const QString& imageFilePath)
	{
		if (!textEditor_)
		{
			return;
		}

		QTextImageFormat imageFormat;
		imageFormat.setWidth(100);
		imageFormat.setHeight(100);
		imageFormat.setName(imageFilePath);
		textEditor_->insertImage(imageFormat);
	}

	void onImageClicked(const QUrl& imageUrl)
	{
		const QString& fileUrl = imageUrl.toString();
		if (!imageUrl.isValid())
		{
			return;
		}
		QDesktopServices::openUrl(fileUrl);
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
