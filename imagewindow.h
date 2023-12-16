#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "stlall.h"				// STL

#include <ui_imagewindow.h>

#include <QApplication>				// Standard QT classes
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
#include <QSlider>
#include <QProgressBar>
#include <QPalette>
#include <QBrush>

#include "renderer.h"
#include "operators.h"

using namespace std;											// Standard namespace

class eScrollArea : public QScrollArea {
	Q_OBJECT
	
	public:
					eScrollArea		(QWidget *parent=0):QScrollArea(parent) {}
		void			scrollBy		(int x, int y) { 
			horizontalScrollBar()	->setSliderPosition(horizontalScrollBar()	->sliderPosition()+x); 
			verticalScrollBar()	->setSliderPosition(verticalScrollBar()		->sliderPosition()+y); 
//cout << "SCROLL " << x << " " << y << endl; scrollContentsBy(x,y); 
		}

	signals:
		void			resized			(QSize);


	private:
		//bool			viewportEvent	(QEvent *e) { if (e && (e->type()==QEvent::Resize)) emit(resized(((QResizeEvent*)e)->size())); return(true); }
};

class imagewindow : public QWidget, private Ui::imagewindow {

	Q_OBJECT

	public:
					imagewindow		(QWidget *parent=0, Qt::WindowFlags f=Qt::WindowFlags());
					imagewindow		(QImage &im, QWidget *parent=0, Qt::WindowFlags f=Qt::WindowFlags());
				       ~imagewindow		();
		bool			isEmpty			() { if (w) return(false); return(true); }
		void			setImage		(QImage);
		void			setContentsPos		(int,int);
		void			setTopVisible		(bool);
		void			addSaveFormat		(QString);
		unsigned long long&	opMask			();
		void			copyTree		(treeNode*);
		void			start			();
		void			stop			();
		void			setAlpha		(bool);
		void			setResolution		(int,int);
		void			setRange		(double,double,double,double);
	signals:
		void			closed			(imagewindow*,int,int);
		void			treeSelected		(treeNode*);
		void			contentsHaveMoved	(int,int);
		void			saveRequest		(QImage&,QString,int);
		void			settingsChanged		(int,int);

	private:
		void			closeEvent		(QCloseEvent *);
		void			mousePressEvent		(QMouseEvent*);
		void			mouseMoveEvent		(QMouseEvent*);
		void			mouseReleaseEvent	(QMouseEvent*);
		void			setup			();
		
		eScrollArea		*sv;
		QLabel			*w;
		QTimer			 mt;
		bool			drag_start,  emitlock;
		int			drag_startx, drag_starty, mx, my;
		renderer		*r;
		treeNode		*t;
		bool			alpha;
		QTimer			timer, timer2;
        	unsigned long long	op_mask;
	
	private slots:
		void			saveSlot		();
		void			scrollViewResizedSlot	(QSize);
		
		void			settingsChangedSlot	(int);
		void			doneSlot		();
		void			timerSlot		();
		void			timer2Slot		();
		void			selectSlot		();
};

#endif

