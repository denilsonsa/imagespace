// ---------------------------------------------------------------------------------
// GENESIS 1.0
// Copyright: SubSymbolics
// Mar-Jul 2003
// ---------------------------------------------------------------------------------
// The render window creates the final version of a selected function


#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <stdlib.h>											// ANSI C   std. functions
#include <stdio.h>											// ANSI C   i/o functions
#include <iostream.h>											// ANSI C++ stream library
#include <fstream.h>											// ANSI C++ File streams

using namespace std;											// Standard namespace

#include <renderwindow_base.h>

#include <qapplication.h>										// Standard QT classes
#include <qobject.h>
#include <qwidget.h>
#include <qevent.h>
#include <qpainter.h>
#include <qscrollview.h>
#include <qprogressbar.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qgroupbox.h>
#include <qbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qstring.h>
#include <qcolor.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qpoint.h>
#include <qmenubar.h>
#include <qtimer.h>
#include <qcombobox.h>

#include "operators.h"											// The syntax tree and vector declarations
#include "renderer.h"											// The renderers create a QImage from a syntax tree
#include "images.h"											// Embedded graphics

class renderwindow : public renderwindow_base {

	Q_OBJECT

	public:
			renderwindow	(QWidget *parent=0, const char *name=0, WFlags f=0);		// Constructor
		       ~renderwindow	();								// Destructor

		void	copyTree	(treeNode*);
		void	start		();
		void	stop		();
		void	setAlpha	(bool);
		void	setResolution	(int,int);
		void	setRange	(double,double,double,double);

	signals:
		void	closed		(renderwindow*);
		void	select		(treeNode*);
	private:
		
		void	closeEvent	(QCloseEvent *);
		
		renderer	*r;
		treeNode	*t;
		QScrollView	*sv;
		QLabel		*w;
		QTimer		timer;
		bool		alpha;
	private slots:
		
		void	doneSlot	();
		void	timerSlot	();
		void	saveSlot	();
		void	selectSlot	();
};

#endif

