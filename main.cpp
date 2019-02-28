#include "cmainwindow.h"
#include <QApplication>
#include <QSettings>

#include "csplashscreen.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationDisplayName("qtIPRange");
	a.setOrganizationName("WIN-DESIGN");
	a.setOrganizationDomain("windesign.at");
	a.setApplicationName("qtIPRange");

	QSettings		settings;

	QPixmap			pixmap(":/images/splash.png");
	QFont			splashFont;
	cSplashScreen*	lpSplash	= new cSplashScreen(pixmap, splashFont);

	lpSplash->setMessageRect(QRect(0, 220, 400, 30));
	lpSplash->show();
	a.processEvents();

	lpSplash->showStatusMessage(QObject::tr("<center><big><big>initializing...</big></big></denter>"));

	cMainWindow w(lpSplash);

	if(settings.value("main/maximized").toBool())
		w.showMaximized();
	else
		w.show();

	lpSplash->finish(&w);
	delete lpSplash;

	return a.exec();
}
