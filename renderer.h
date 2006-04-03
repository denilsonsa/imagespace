// ----------------------------------------------------------------------------
// SubSymbolics 2003
// Jul 2003
// ----------------------------------------------------------------------------

#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>								// STD and STL classes
#include <stdlib.h>
#include "stlall.h"

#include <QObject>								// QT classes
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QThread>
#include <QMutex>

#include "operators.h"

using namespace std;								// STD namespace

class renderer : public QThread {					// Renderer object

	Q_OBJECT

	public:
				 	renderer	(QObject * parent=0 , const char * name=0, int w=64, int h=64);
				       ~renderer	();
		void		 	setTree		(treeNode *t);
		bool		 	setRange	(double xmin=0.0, double xmax=1.0, double ymin=0.0, double ymax=1.0);
		void		 	setResolution	(int,int);
		bool		 	runs		();
	virtual void		 	run		();
		void		 	requestStop	();
		void		 	setAlpha	(bool);
		QImage*		 	image		();
		int		 	line		();
		unsigned long long&	opMask		();
	signals:		
		void		 	lineDone	(int);
		void		 	done		();
	private:
		void		 	setPixel	(unsigned int, unsigned int, D4&);
		QImage		 	im;
		treeNode	       *tn;
		double		 	x0,y0,x1,y1;
		bool		 	stop_flag, runs_flag, alpha;
		QMutex		 	mutex;
		int		 	line_counter, width, height;
		unsigned long long	opmask;

};
#endif
