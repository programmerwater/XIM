#ifndef MESSAGE_H
#define MESSAGE_H

#include "QtCore/QMetaType"
#include "QtCore/QString"

enum class MessageType
{
	Unknown,
	User,
	System,
};
Q_DECLARE_METATYPE(MessageType);

struct MessageFormat 
{
	QString content;
};

struct MessageItem 
{
	bool isSelf = false;
	MessageFormat messageFormat;
	quint64 time = 0;
	QString senderName;
	QString receiverName;
};

#endif // !MESSAGE_H
