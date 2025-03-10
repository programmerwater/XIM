#ifndef CHATTEXTEDITORTOOL_H
#define CHATTEXTEDITORTOOL_H

#include "QtWidgets/QWidget"

class ChatTextEditorToolPrivate;

class ChatTextEditorTool final: public QWidget
{
    Q_OBJECT

public:
    explicit ChatTextEditorTool(QWidget *parent = 0);
    ~ChatTextEditorTool();

	Q_SIGNAL void insertImage(const QString& imageFilePath, QPrivateSignal);

private:
	Q_DECLARE_PRIVATE(ChatTextEditorTool);
	Q_DISABLE_COPY(ChatTextEditorTool);
	QScopedPointer<ChatTextEditorToolPrivate> d_ptr;
};

#endif // CHATTEXTEDITORTOOL_H
