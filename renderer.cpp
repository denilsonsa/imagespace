#include "renderer.h"

renderer::renderer( QObject * parent , const char * name , int w, int h) : QObject(parent,name), QThread() {
	tn=NULL;
	runs_flag=false;
	alpha=true;
	x0=0;
	x1=1;
	y0=0;
	y1=1;
	if (w<1) w=1;
	if (h<1) h=1;
	im=QImage(w,h,32);	
}

renderer::~renderer() { }

void renderer::setAlpha(bool a) { alpha=a; }

void renderer::setTree(treeNode *t) { tn=t; }

void renderer::requestStop() { stop_flag=true; }

bool renderer::runs() { return(runs_flag); }

QImage* renderer::image() { return(&im); }

void renderer::setPixel(unsigned int w, unsigned int h, D4 &clr) {
	unsigned char 	r=(unsigned char)(((clr[0]+1.0)/2.0)*255.0),
			g=(unsigned char)(((clr[1]+1.0)/2.0)*255.0),
			b=(unsigned char)(((clr[2]+1.0)/2.0)*255.0),
			a=(unsigned char)(((clr[3]+1.0)/2.0)*255.0);
	if (r>255) cout << "R " << r << endl;
	if (g>255) cout << "G " << g << endl;
	if (b>255) cout << "B " << b << endl;
	QRgb c=qRgba(r,g,b,a);	
	im.setPixel(w,h,c);
}

void renderer::run() {
	if (tn!=NULL) {
		im.setAlphaBuffer(alpha);
		im.fill(qRgba(0,0,0,0));		
		D4 clr;
		double  sx=(x1-x0)/(double)im.width (),
			sy=(y1-y0)/(double)im.height();
		stop_flag=false;
		runs_flag=true;
		for (int h=0;h<im.height();++h) if (!stop_flag) {
			double y=y0+sy*(double)h;
			for (int w=0;w<im.width();++w) if (!stop_flag) {
				double x=x0+sx*(double)w;
				clr=tn->get(x,y);
				setPixel(w,h,clr);
			}
		}
	}
	runs_flag=false;
}

void renderer::setResolution(int width, int height) {
	im=QImage(width,height,32);
}

bool renderer::setRange(double xmin, double xmax, double ymin, double ymax) {
	if ((xmin<xmax) && (ymin<ymax)) {
		x0=xmin;
		x1=xmax;
		y0=ymin;
		y1=ymax;
		return(true);
	}
	return(false);
}


