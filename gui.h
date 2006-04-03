// ---------------------------------------------------------------------------------
// ImageSpace
// QT4 Version Mar 2006
// ---------------------------------------------------------------------------------

#ifndef GUI_H
#define GUI_H

using namespace std;								// Standard namespace

#include <stdlib.h>								// ANSI C   std. functions
#include <stdio.h>								// ANSI C   i/o functions
#include "stlall.h"
#include <ui_gui.h>								// The GUI headers created by QDesigner
#include "imagewindow.h"

#include <QApplication>								// Standard QT classes
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QCloseEvent>
#include <QPainter>
#include <QScrollArea>
#include <QProgressBar>
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QColor>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QMenuBar>
#include <QTimer>
#include <QPicture>
#include <QStringList>
#include <QComboBox>
#include <QScrollBar>
#include <QList>
#include <QByteArray>
#include <QSettings>
#include <QVBoxLayout>
#include <QPalette>
#include <QBrush>

#include "operators.h"
#include "renderer.h"

class imLabel : public QLabel {
	Q_OBJECT
	public:
			imLabel		(QWidget *parent=0, Qt::WFlags f=0):QLabel(parent,f) {
				setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
			} 
		void	mousePressEvent	(QMouseEvent *e) {
			if (e) emit(clicked());
		}

	signals:	
		void	clicked		();
};

class gui : public QWidget, private Ui::gui {

    Q_OBJECT

	public:
			 gui		(QWidget *parent=0, Qt::WFlags f=0);	// Constructor
			~gui		();					// Destructor

	private slots:
		void	loadSlot		();
		void	saveSlot		();
		void	saveImageSlot		(QImage&,QString,int);
		void	checkDone		();

		void	g1Slot			();				// If a genome was selected, the buttons call these slots
		void	g2Slot			();
		void	g3Slot			();
		void	g4Slot			();
		void	g5Slot			();
		void	g6Slot			();
		void	g7Slot			();
		void	g8Slot			();
		void	g9Slot			();

		void	rend1DoneSlot		();
		void	rend2DoneSlot		();
		void	rend3DoneSlot		();
		void	rend4DoneSlot		();
		void	rend5DoneSlot		();
		void	rend6DoneSlot		();
		void	rend7DoneSlot		();
		void	rend8DoneSlot		();
		void	rend9DoneSlot		();

		void	t1Slot			();
		void	t2Slot			();
		void	t3Slot			();
		void	t4Slot			();
		void	t5Slot			();
		void	t6Slot			();
		void	t7Slot			();
		void	t8Slot			();
		void	t9Slot			();

		void	x0Slot			(int);
		void	x1Slot			(int);
		void	y0Slot			(int);
		void	y1Slot			(int);

		void	renderSlot		();
		void	renderWindowClosed	(imagewindow*,int,int); 
		void	renderWindowSelect	(treeNode*);
		void	alphaSlot		(bool);

		void	tanhSlot		(bool);
		void	cosSlot			(bool);
		void	sinSlot			(bool);
		void	gaussSlot		(bool);
		void	prodSlot		(bool);
		void	qprodSlot		(bool);
		void	addSlot			(bool);
		void	sh0Slot			(bool);
		void	sh1Slot			(bool);
		void	sh2Slot			(bool);
		void	sh3Slot			(bool);
		void	perm01Slot		(bool);
		void	perm02Slot		(bool);
		void	perm03Slot		(bool);
		void	perm12Slot		(bool);
		void	perm13Slot		(bool);
		void	perm23Slot		(bool);

	private:
	
		void	closeEvent		(QCloseEvent*);

		void	selectGenome		(int nr, bool frst=false);	// Select genome #0..8
		void	startRenderers		();				// Start all renderers (if !first, the central renderer is not started)
		void	stopRenderers		();				// Stop all renderers
                void	saveConfig		();
		void	loadConfig		();
		void	displayOpMask		();
		renderer			*rend[9];			// The renderers
		treeNode			*tree[9],			// The genomes (syntax trees containing descriptions of functions)
						*selectedTree;
		QTimer    			t[9];
		bool	  			first;				// Re-render the selected tree (after x0:x1 y0:y1 ranges have changed)
		imagewindow			*lastRenderWindow;
		unsigned long long		opMask;
		imLabel				*g1,*g2,*g3,*g4,*g5,*g6,*g7,*g8,*g9;
		QFileDialog			fd1,fd2,fd3;
		vector<treeNode[9]>		history;
};

#endif
