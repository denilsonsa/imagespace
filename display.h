// ---------------------------------------------------------------------------------
// SubSymbolics 2003
// ---------------------------------------------------------------------------------

#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H

#include <stdlib.h>											// ANSI C std. functions
#include <stdio.h>											// ANSI C i/o functions
#include "stlall.h"											// STL

#include <qapplication.h>										// Standard QT classes
#include <qwidget.h>
#include <qobject.h>
#include <qptrlist.h>
#include <qobjectlist.h>
#include <qscrollview.h>
#include <qlabel.h>
#include <qbutton.h>
#include <qpushbutton.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qevent.h>

using namespace std;											// Standard namespace

class display : public QScrollView {									// The background widget of a page manages the graphics, i.e. it provides a QT widget for every basicWidget object

	Q_OBJECT

	public:
			display			(QWidget *parent=0, const char *name=0, WFlags f=0);	// Constructor

		void	setResolution		(unsigned int h=512, unsigned int v=512);

	private:
		QWidget *w;

};

#endif
