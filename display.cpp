#include "display.h"

display::display(QWidget *parent, const char *name, WFlags f):QScrollView(parent,name,f) {
	w=NULL;
	setResolution();
}

void display::setResolution(unsigned int h, unsigned int v) {
	if (w!=NULL) delete(w);
	w=new QWidget(this);
	w->resize(h,v);
	w->show();
}

