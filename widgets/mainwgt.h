#ifndef MAINWGT_H
#define MAINWGT_H

#include "QtWidgets/QWidget"

class MainWgtPrivate;

class MainWgt : public QWidget
{
	Q_OBJECT

public:
	MainWgt(QWidget* parent = nullptr);
	~MainWgt();

private:
	Q_DECLARE_PRIVATE(MainWgt);
	Q_DISABLE_COPY(MainWgt);

	QScopedPointer<MainWgtPrivate> d_ptr;
};

#endif // MAINWGT_H
