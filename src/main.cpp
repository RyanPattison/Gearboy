#include <QApplication>
#include <QQuickView>

#include "GBEmulator.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("gearboy");
    app.setOrganizationDomain("rpattison.com");
    app.setApplicationName("GearBoy");

	qmlRegisterType<GBEmulator>("GearBoy", 1, 0, "GearBoyEmulator");

	QQuickView view;
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl("qrc:///main.qml"));
	view.show();

	int result = app.exec();
	EmulationRunner::waitAll();	
	return result;
}
