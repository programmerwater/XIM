#ifndef MESSAGE_H
#define MESSAGE_H

#include "QtCore/QJsonArray"
#include "QtCore/QMetaType"
#include "QtCore/QString"

enum class MessageType
{
	Unknown,
	User,
	System,
};
Q_DECLARE_METATYPE(MessageType);

struct MessageItem 
{
	bool isSelf = false;
	quint64 time = 0;
	QJsonArray msgTypes;
	QString senderName;
	QString receiverName;
};

#endif // !MESSAGE_H
