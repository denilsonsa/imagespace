#include "renderwindow.h"

renderwindow::renderwindow(QWidget *parent, const char *name, WFlags f):renderwindow_base(parent,name,f) {
	r=new renderer(this);
	t=NULL;

	connect( r         ,SIGNAL(    done   ()),this,SLOT(    doneSlot   ()));
	connect(&timer     ,SIGNAL( timeout   ()),this,SLOT(   timerSlot   ()));
	connect( saveButton,SIGNAL( clicked   ()),this,SLOT(    saveSlot   ()));

	QBoxLayout *boxLayout=new QBoxLayout(displayFrame,QBoxLayout::Up);
	boxLayout->setAutoAdd(true);

	sv=new QScrollView(displayFrame);
	w =new QLabel(sv);
	sv->show();
	
	w ->resize(256,256);
	sv->addChild(w);
	w ->show();
}

renderwindow::~renderwindow() {
}

void renderwindow::closeEvent(QCloseEvent * e) {
	if (timer.isActive()) timer.stop();
	if (r!=NULL) {
		if (r->running()) r->requestStop();
		do {} while (r->running());
		delete r;
	}

	if (t!=NULL) delete t;

	qApp->lock();
	emit(closed(this));
	qApp->unlock();
	e->accept();
}

void renderwindow::copyTree(treeNode *tn) {
	stop();
	if (t!=NULL) delete t;
	t=tn->copy();
}

void renderwindow::start() {
	stop();
	if (t!=NULL) {
		r->setTree(t);
		r->setResolution(w->width(),w->height());
		if (alpha) w->setPaletteBackgroundPixmap(QImage(qembed_findData("alpha.png")));
		      else w->setPaletteBackgroundColor(QColor(0,0,0));
		r->start();
		timer.start(2000);
	}
}

void renderwindow::doneSlot() { w->setPixmap(*(r->image())); timer.stop(); }

void renderwindow::setResolution(int h, int v) {
	stop();
	w->resize(h,v);
}

void renderwindow::setRange(double x0, double x1, double y0, double y1) {
	stop();
	r->setRange(x0,x1,y0,y1);
}

void renderwindow::stop() {
	if (r->running()) {
		r->requestStop();
		timer.stop();
		do {} while (r->running());
	}
}

void renderwindow::timerSlot() {
	bool d;
	timer.stop();
	if (!r->running()) d=true; else d=false;
	w->setPixmap(*(r->image()));
	if (!d) timer.start(2000);
}

void renderwindow::saveSlot() {
	QString s=QFileDialog::getSaveFileName("", "",this, "save file dialog","Choose a file" );
	if (s!="") {
		stop();
		r->image()->save(s,"PNG");
	}
}

void renderwindow::setAlpha(bool a) {
	r->setAlpha(a);
	alpha=a;
}
