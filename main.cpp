#include "QtWidgets/QApplication"

#include "xframelesswidget.h"
#include "widgets/mainwgt.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	MainWgt mainContentWgt;
	XFramelessWidgetWithCaption mainWnd;
	mainWnd.setWindowTitle("XIM");
	QObject::connect(&mainWnd, SIGNAL(closeRequested()), &app, SLOT(quit()));
	mainWnd.setContentWidget(&mainContentWgt);
	mainWnd.resize(400, 800);
	mainWnd.showCenter();

	return app.exec();
}
