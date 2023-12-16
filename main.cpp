#include "gui.h"				// Control object header
#include "stlall.h"

#include <stdlib.h>

#include <QApplication>				// QT
#include <QObject>
#include <QPushButton>
#include <QDialog>
#include <QWidget>
#include <QSplashScreen>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QFont>
#include <QSplashScreen>

using namespace std;				// Use standard namespace

int main(int argc, char **argv) {
	srand(time(NULL));
	QApplication app(argc,argv);
	gui *w=new gui(0);
	QSplashScreen *s=new QSplashScreen(QPixmap(":/images/splash.jpg"));
	QString msg="Version 0.5\nCompiled ";
	msg+=__DATE__;
	s->setFont(QFont("Helvetica",14,75));
	s->showMessage(msg,Qt::AlignRight|Qt::AlignBottom,QColor(255,255,255));
	QTimer *t=new QTimer(s);
	QObject::connect(t,SIGNAL(timeout()),s,SLOT(close()));
	QObject::connect(t,SIGNAL(timeout()),w,SLOT(show ()));
	t->start(2000);
	s->show();
	return app.exec();
};
