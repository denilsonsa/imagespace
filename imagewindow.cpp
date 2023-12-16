#include "imagewindow.h"

imagewindow::imagewindow(QWidget *parent, Qt::WindowFlags f):QWidget(parent,f) {
	setup();
}

imagewindow::imagewindow(QImage &im, QWidget *parent, Qt::WindowFlags f):QWidget(parent,f) {
	setup();
	setImage(im);
}

void imagewindow::addSaveFormat(QString s) {
	format->addItem(s);
}

void imagewindow::setTopVisible(bool b) {
	if (b) top_group->show(); else top_group->hide();
}

unsigned long long& imagewindow::opMask() { return(op_mask); }

void imagewindow::setup() {
	op_mask=~0;
	setupUi(this);
	
	r=new renderer(this);
	t=NULL;
	connect( r           ,SIGNAL(   done()),this,SLOT(  doneSlot()));
	connect(&timer       ,SIGNAL(timeout()),this,SLOT( timerSlot()));
	connect(&timer2      ,SIGNAL(timeout()),this,SLOT(timer2Slot()));
	connect( selectButton,SIGNAL(clicked()),this,SLOT(selectSlot()));

	drag_start=false;
	QBoxLayout *boxLayout=new QBoxLayout(QBoxLayout::Up,displayFrame);
	sv=new eScrollArea(displayFrame);	
	boxLayout->addWidget(sv);
	emitlock=false;
	w=NULL;
	sv->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); 
	sv->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); 
	sv->show();
	
	connect(saveButton,	SIGNAL(clicked()),		this,	SLOT(			  saveSlot()));
	connect(sv,		SIGNAL(resized(QSize)),		this,	SLOT(	scrollViewResizedSlot(QSize)));
	connect(format,		SIGNAL(activated(int)),		this,	SLOT(	    settingsChangedSlot(int)));
	
	addSaveFormat("PNG");
	addSaveFormat("JPG");
	settingsChangedSlot(0);
}

imagewindow::~imagewindow() {
}

void imagewindow::settingsChangedSlot(int i) {
	emit(settingsChanged(format->currentIndex(),-1));
}

void imagewindow::setImage(QImage im) {
	if (!w) {
		w=new QLabel(sv);
		QPalette palette;
		palette.setBrush(w->backgroundRole(), QBrush(QPixmap(":/images/alpha.png")));
		w->setPalette(palette);
		sv->setWidget(w);
	}
	w->resize(im.width(),im.height());
	w->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	w->setPixmap(QPixmap::fromImage(im));
	w->show();
}

void imagewindow::closeEvent(QCloseEvent * e) {
	if (e) {
		if (timer .isActive()) timer .stop();
		if (timer2.isActive()) timer2.stop();
		if (r) {
			if (r->isRunning()) r->requestStop();
			do {} while (r->isRunning());
			delete r;
		} 
		if (t) delete t;
		emit(closed(this,width(),height()));
		e->accept();
	}
}

void imagewindow::saveSlot() {
	if (w && w->pixmap()) {
		QImage im=w->pixmap()->toImage();
		emit(saveRequest(im,format->currentText(),-1));
	}
}

void imagewindow::mouseMoveEvent(QMouseEvent *e) {
	if (e && drag_start && sv) {
		int	dx=drag_startx-e->globalX(),
			dy=drag_starty-e->globalY();
		sv->scrollBy(dx,dy);
		drag_startx=e->globalX();
		drag_starty=e->globalY();
	}
}

void imagewindow::mouseReleaseEvent(QMouseEvent *e) {
	if (e) drag_start=false;	
}

void imagewindow::mousePressEvent(QMouseEvent *e) {
	if (e && w) {
		drag_start=true;
		drag_startx=e->globalX();
		drag_starty=e->globalY();
	}
}

void imagewindow::scrollViewResizedSlot(QSize s) {
	if (w && (w->pixmap()->height()<s.height())) w->setFixedHeight(s.height());
	if (w && (w->pixmap()->width ()<s.width ())) w->setFixedWidth (s.width ());
}


void imagewindow::copyTree(treeNode *tn) {
	stop();
	if (t!=NULL) delete t;
	t=tn->copy();
}

void imagewindow::start() {
	stop();
	if (t && r) {
		r->setTree(t);
		r->opMask()=op_mask;
		r->setAlpha(alpha);
		r->start();
		timer.start(1000);
		timer2.start(100);
	}
}

void imagewindow::doneSlot() { 
	setImage(*(r->image()));
	timer.stop();
}

void imagewindow::setResolution(int h, int v) {
	stop();
	if (r) r->setResolution(h,v);
	progress->setMaximum(v);
}

void imagewindow::setRange(double x0, double x1, double y0, double y1) {
	stop();
	if (r) r->setRange(x0,x1,y0,y1);
}

void imagewindow::stop() {
	if (r && r->isRunning()) {
		r->requestStop();
		timer.stop();
		do {} while (r->isRunning());
	}
}

void imagewindow::timer2Slot() {
	if (timer.isActive()) {
		if (r && r->isRunning()) progress->setValue(r->line()); 
		else	progress->reset();
	} else {
		timer2.stop();
		progress->reset();
		timerSlot();
	}
}

void imagewindow::timerSlot() {
	if (r) {
		setImage(*(r->image()));
		if (!r->isRunning()) timer.stop();
	}
}

void imagewindow::setAlpha(bool a) {
	if (r) {
		r->setAlpha(a);
		alpha=a;
	}
}

void imagewindow::selectSlot() {
	if (t) emit(treeSelected(t));
}

