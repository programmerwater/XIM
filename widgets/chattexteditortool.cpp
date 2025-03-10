#include "chattexteditortool.h"

#include "QtCore/QStandardpaths"
#include "QtWidgets/QFileDialog"
#include "QtWidgets/QHBoxLayout"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QSizePolicy"

class ChatTextEditorToolPrivate final: public QObject
{
public:
	ChatTextEditorToolPrivate(ChatTextEditorTool* q)
		: q_ptr(q)
	{
	}

	~ChatTextEditorToolPrivate()
	{
	}

	void init()
	{
		Q_Q(ChatTextEditorTool);
		QPushButton* imageBtn = new QPushButton(q);
		QHBoxLayout* hLayout = new QHBoxLayout(q);
		hLayout->setContentsMargins(0, 0, 0, 0);
		hLayout->addWidget(imageBtn);
		hLayout->addSpacerItem(new QSpacerItem(0, 0, 
			QSizePolicy::Expanding, QSizePolicy::Minimum));

		QObject::connect(imageBtn, &QPushButton::clicked, 
			this, &ChatTextEditorToolPrivate::onImageBtnClicked);
	}

	void onImageBtnClicked()
	{
		Q_Q(ChatTextEditorTool);
		const QString& downloadsPath = QStandardPaths::writableLocation(
			QStandardPaths::DownloadLocation);
		const QString& imageFilePath = QFileDialog::getOpenFileName(q, 
			tr("Select Image"), downloadsPath, "Image Files(*.jpg *.png *.bmp *.jpeg)");
		if (QUrl::fromLocalFile(imageFilePath).isValid())
		{
			emit q->insertImage(imageFilePath, ChatTextEditorTool::QPrivateSignal());
		}	
	}

private:
	Q_DECLARE_PUBLIC(ChatTextEditorTool);
	ChatTextEditorTool* q_ptr;
};

ChatTextEditorTool::ChatTextEditorTool(QWidget *parent)
	: QWidget(parent)
	, d_ptr(new ChatTextEditorToolPrivate(this))
{
	Q_D(ChatTextEditorTool);
	d->init();
}


ChatTextEditorTool::~ChatTextEditorTool()
{

}
