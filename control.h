// ---------------------------------------------------------------------------------
// ImageSpace
// Mar-Jul 2003
// ---------------------------------------------------------------------------------
// The Control Object implements the user interface, inherited from QMainWindow


#ifndef CONTROL_H
#define CONTROL_H

using namespace std;											// Standard namespace

#include <stdlib.h>											// ANSI C   std. functions
#include <stdio.h>											// ANSI C   i/o functions
#include <iostream.h>											// ANSI C++ stream library
#include <fstream.h>											// ANSI C++ File streams

#include <gui.h>											// The GUI headers created by QDesigner

#include <qapplication.h>										// Standard QT classes
#include <qpainter.h>
#include <qprogressbar.h>
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
#include <qtimer.h>
#include <qmenubar.h>

#include "images.h"											// Embedded graphics
#include "operators.h"											// The syntax tree and vector declarations
#include "renderer.h"											// The renderers create a QImage from a syntax tree
#include "renderwindow.h"

class control : public MainWindow {									// This is the MainWidget

    Q_OBJECT

	public:
			 control	(QWidget *parent=0, const char *name=0, WFlags f=0);		// Constructor
			~control	();								// Destructor

	private slots:
		void	loadSlot	();
		void	saveSlot	();
		void	checkDone	();
		
		void	g1Slot		();								// If a genome was selected, the buttons call these slots
		void	g2Slot		();
		void	g3Slot		();
		void	g4Slot		();
		void	g5Slot		();
		void	g6Slot		();
		void	g7Slot		();
		void	g8Slot		();
		void	g9Slot		();

		void	rend1DoneSlot	();
		void	rend2DoneSlot	();
		void	rend3DoneSlot	();
		void	rend4DoneSlot	();
		void	rend5DoneSlot	();
		void	rend6DoneSlot	();
		void	rend7DoneSlot	();
		void	rend8DoneSlot	();
		void	rend9DoneSlot	();
		
		void	t1Slot		();
		void	t2Slot		();
		void	t3Slot		();
		void	t4Slot		();
		void	t5Slot		();
		void	t6Slot		();
		void	t7Slot		();
		void	t8Slot		();
		void	t9Slot		();

		void	x0Slot		(int);
		void	x1Slot		(int);
		void	y0Slot		(int);
		void	y1Slot		(int);

		void	renderSlot	();								// Start a new main renderer
		void	renderWindowClosed(renderwindow*);
		void	alphaSlot	(bool);
	private:
	
		void closeEvent			(QCloseEvent*);

		void selectGenome		(int nr, bool frst=false);				// Select genome #0..8
		void startRenderers		();							// Start all renderers (if !first, the central renderer is not started)
		void stopRenderers		();							// Stop all renderers

		renderer  *rend[9];									// The renderers
		treeNode  *tree[9];									// The genomes (syntax trees containing descriptions of functions)
		treeNode  *selectedTree;
		QTimer    t[9];
		bool	  first;									// Re-render the selected tree (after x0:x1 y0:y1 ranges have changed)
		renderwindow *lastRenderWindow;
};

#endif
