#include "gui.h"

gui::gui(QWidget *parent, Qt::WFlags f):QWidget(parent,f) {

	setupUi(this);

	opMask=~0;
	displayOpMask();
	lastRenderWindow=NULL;

	for (int i=0;i<9;++i) {
		rend[i]=new renderer(this,0);
		rend[i]->setAlpha(alpha->isChecked());
		tree[i]=new treeNode(false,1.0,(double)decay->value()/100.0,maxSubnodes->value(),(double)mutation->value()/100.0);
	}

	connect(x0,SIGNAL(valueChanged(int)),this,SLOT(x0Slot(int)));
	connect(x1,SIGNAL(valueChanged(int)),this,SLOT(x1Slot(int)));
	connect(y0,SIGNAL(valueChanged(int)),this,SLOT(y0Slot(int)));
	connect(y1,SIGNAL(valueChanged(int)),this,SLOT(y1Slot(int)));

	selectedTree=tree[4];
	connect(&t[0],SIGNAL(timeout()),this,SLOT(t1Slot()));
	connect(&t[1],SIGNAL(timeout()),this,SLOT(t2Slot()));
	connect(&t[2],SIGNAL(timeout()),this,SLOT(t3Slot()));
	connect(&t[3],SIGNAL(timeout()),this,SLOT(t4Slot()));
	connect(&t[4],SIGNAL(timeout()),this,SLOT(t5Slot()));
	connect(&t[5],SIGNAL(timeout()),this,SLOT(t6Slot()));
	connect(&t[6],SIGNAL(timeout()),this,SLOT(t7Slot()));
	connect(&t[7],SIGNAL(timeout()),this,SLOT(t8Slot()));
	connect(&t[8],SIGNAL(timeout()),this,SLOT(t9Slot()));

	g1f->setLayout(new QVBoxLayout());	g1=new imLabel(g1f);	g1f->layout()->addWidget(g1);	g1f->layout()->setMargin(1);	g1f->layout()->setSpacing(1);
	g2f->setLayout(new QVBoxLayout());	g2=new imLabel(g2f);	g2f->layout()->addWidget(g2);	g2f->layout()->setMargin(1);	g2f->layout()->setSpacing(1);
	g3f->setLayout(new QVBoxLayout());	g3=new imLabel(g3f);	g3f->layout()->addWidget(g3);	g3f->layout()->setMargin(1);	g3f->layout()->setSpacing(1);
	g4f->setLayout(new QVBoxLayout());	g4=new imLabel(g4f);	g4f->layout()->addWidget(g4);	g4f->layout()->setMargin(1);	g4f->layout()->setSpacing(1);
	g5f->setLayout(new QVBoxLayout());	g5=new imLabel(g5f);	g5f->layout()->addWidget(g5);	g5f->layout()->setMargin(1);	g5f->layout()->setSpacing(1);
	g6f->setLayout(new QVBoxLayout());	g6=new imLabel(g6f);	g6f->layout()->addWidget(g6);	g6f->layout()->setMargin(1);	g6f->layout()->setSpacing(1);
	g7f->setLayout(new QVBoxLayout());	g7=new imLabel(g7f);	g7f->layout()->addWidget(g7);	g7f->layout()->setMargin(1);	g7f->layout()->setSpacing(1);
	g8f->setLayout(new QVBoxLayout());	g8=new imLabel(g8f);	g8f->layout()->addWidget(g8);	g8f->layout()->setMargin(1);	g8f->layout()->setSpacing(1);
	g9f->setLayout(new QVBoxLayout());	g9=new imLabel(g9f);	g9f->layout()->addWidget(g9);	g9f->layout()->setMargin(1);	g9f->layout()->setSpacing(1);
	

	QPalette palette;
	palette.setBrush(g1->backgroundRole(), QBrush(QPixmap(":/images/alpha.png")));
	g1->setPalette(palette);
	g2->setPalette(palette);
	g3->setPalette(palette);
	g4->setPalette(palette);
	g5->setPalette(palette);
	g6->setPalette(palette);
	g7->setPalette(palette);
	g8->setPalette(palette);
	g9->setPalette(palette);

	connect(g1,SIGNAL(clicked()),this,SLOT(g1Slot())); 
	connect(g2,SIGNAL(clicked()),this,SLOT(g2Slot()));
	connect(g3,SIGNAL(clicked()),this,SLOT(g3Slot()));
	connect(g4,SIGNAL(clicked()),this,SLOT(g4Slot()));
	connect(g5,SIGNAL(clicked()),this,SLOT(g5Slot()));
	connect(g6,SIGNAL(clicked()),this,SLOT(g6Slot()));
	connect(g7,SIGNAL(clicked()),this,SLOT(g7Slot()));
	connect(g8,SIGNAL(clicked()),this,SLOT(g8Slot()));
	connect(g9,SIGNAL(clicked()),this,SLOT(g9Slot()));

	connect(     op_tanh,SIGNAL(toggled(bool)),this,SLOT(  tanhSlot(bool)));
	connect(      op_cos,SIGNAL(toggled(bool)),this,SLOT(   cosSlot(bool)));
	connect(      op_sin,SIGNAL(toggled(bool)),this,SLOT(   sinSlot(bool)));
	connect(    op_gauss,SIGNAL(toggled(bool)),this,SLOT( gaussSlot(bool)));
	
	connect(     op_prod,SIGNAL(toggled(bool)),this,SLOT(  prodSlot(bool)));
	connect(    op_qprod,SIGNAL(toggled(bool)),this,SLOT( qprodSlot(bool)));
	connect(      op_sum,SIGNAL(toggled(bool)),this,SLOT(   addSlot(bool)));

	connect(      op_sh0,SIGNAL(toggled(bool)),this,SLOT(   sh0Slot(bool)));
	connect(      op_sh1,SIGNAL(toggled(bool)),this,SLOT(   sh1Slot(bool)));
	connect(      op_sh2,SIGNAL(toggled(bool)),this,SLOT(   sh2Slot(bool)));
	connect(      op_sh3,SIGNAL(toggled(bool)),this,SLOT(   sh3Slot(bool)));
	
	connect(   op_perm01,SIGNAL(toggled(bool)),this,SLOT(perm01Slot(bool)));
	connect(   op_perm02,SIGNAL(toggled(bool)),this,SLOT(perm02Slot(bool)));
	connect(   op_perm03,SIGNAL(toggled(bool)),this,SLOT(perm03Slot(bool)));
	connect(   op_perm12,SIGNAL(toggled(bool)),this,SLOT(perm12Slot(bool)));
	connect(   op_perm13,SIGNAL(toggled(bool)),this,SLOT(perm13Slot(bool)));
	connect(   op_perm23,SIGNAL(toggled(bool)),this,SLOT(perm23Slot(bool)));

	connect(rend[0],SIGNAL(done()),this,SLOT(rend1DoneSlot()));
	connect(rend[1],SIGNAL(done()),this,SLOT(rend2DoneSlot()));
	connect(rend[2],SIGNAL(done()),this,SLOT(rend3DoneSlot()));
	connect(rend[3],SIGNAL(done()),this,SLOT(rend4DoneSlot()));
	connect(rend[4],SIGNAL(done()),this,SLOT(rend5DoneSlot()));
	connect(rend[5],SIGNAL(done()),this,SLOT(rend6DoneSlot()));
	connect(rend[6],SIGNAL(done()),this,SLOT(rend7DoneSlot()));
	connect(rend[7],SIGNAL(done()),this,SLOT(rend8DoneSlot()));
	connect(rend[8],SIGNAL(done()),this,SLOT(rend9DoneSlot()));

	connect(renderButton,SIGNAL(clicked()),this,SLOT(renderSlot()));
	connect(  saveButton,SIGNAL(clicked()),this,SLOT(  saveSlot()));
	connect(  loadButton,SIGNAL(clicked()),this,SLOT(  loadSlot()));
	connect(       alpha,SIGNAL(toggled(bool)),this,SLOT(alphaSlot(bool)));
	first=true;
	loadConfig();
	startRenderers();
}

gui::~gui() {
	for (int i=0;i<9;++i) {
		delete rend[i];
		delete tree[i];
	}
}

void gui::alphaSlot(bool a) { first=true; }

void gui::renderSlot() {
	if ((autoOpen->isChecked()) || !lastRenderWindow) {
		lastRenderWindow=new imagewindow(0);
		connect(lastRenderWindow,SIGNAL(closed(imagewindow*,int,int)),this,SLOT(renderWindowClosed(imagewindow*,int,int)));
		connect(lastRenderWindow,SIGNAL(treeSelected(treeNode*)),this,SLOT(renderWindowSelect(treeNode*)));
		connect(lastRenderWindow,SIGNAL(saveRequest(QImage&,QString,int)),this,SLOT(saveImageSlot(QImage&,QString,int)));
	}

	lastRenderWindow->copyTree(selectedTree);
	lastRenderWindow->setRange(x0->value(),x1->value(),y0->value(),y1->value());
	lastRenderWindow->setResolution(hres->value(),vres->value());
	lastRenderWindow->show();
	lastRenderWindow->setAlpha(alpha->isChecked());
	lastRenderWindow->opMask()=opMask;
	lastRenderWindow->start();
}

void gui::saveSlot() {
	QString s=QFileDialog::getSaveFileName(this,"Choose a filename","./","*.tree" );
	if (s!="") {
		stopRenderers();
		QFileInfo fi(s);
		QDir d=fi.dir();
		QString s2=fi.dir().absolutePath()+"/"+fi.baseName()+".tree";
		tree[4]->save(s2.toAscii().constData());
	} 
}
void gui::loadSlot() {
	QString s=QFileDialog::getOpenFileName(this,"Choose a file" );
	if (s!="") {
		stopRenderers();
		tree[4]->load(s.toAscii().constData());
		opMask=tree[4]->getOpMask();
		displayOpMask();
		selectGenome(4,true);
	} 
}

void gui::closeEvent (QCloseEvent *e)  {
	saveConfig();
	e->accept();
}

void gui::g1Slot() { selectGenome(0,first); }
void gui::g2Slot() { selectGenome(1,first); }
void gui::g3Slot() { selectGenome(2,first); }
void gui::g4Slot() { selectGenome(3,first); }
void gui::g5Slot() { selectGenome(4,first); }
void gui::g6Slot() { selectGenome(5,first); }
void gui::g7Slot() { selectGenome(6,first); }
void gui::g8Slot() { selectGenome(7,first); }
void gui::g9Slot() { selectGenome(8,first); }

void gui::rend1DoneSlot () { checkDone(); }
void gui::rend2DoneSlot () { checkDone(); }
void gui::rend3DoneSlot () { checkDone(); }
void gui::rend4DoneSlot () { checkDone(); }
void gui::rend5DoneSlot () { checkDone(); }
void gui::rend6DoneSlot () { checkDone(); }
void gui::rend7DoneSlot () { checkDone(); }
void gui::rend8DoneSlot () { checkDone(); }
void gui::rend9DoneSlot () { checkDone(); }

void gui::selectGenome(int nr, bool frst) {							// Select a new genome
	first=frst;
	stopRenderers();									// Stop all renderers
	selectedTree=tree[nr];
	if (nr!=4) g5->setPixmap(QPixmap::fromImage(*(rend[nr]->image())));						// Copy the selected pixmap to the central button
	for (int i=0;i<9;++i) if (i!=nr) delete tree[i];					// Delete unselected trees
	for (int i=0;i<9;++i) if (i!=nr) tree[i]=tree[nr]->copy();				// Clone the selected tree nine times
	for (int i=0;i<9;++i) if (i!=4) {							// Mutate all trees with the exception of the central tree
		do { } while (tree[i]->mutate(1.0,(double)decay->value()/100.0,maxSubnodes->value(),(double)mutation->value()/100.0)==0);
	}
	selectedTree=tree[4];
	startRenderers();									// Start all renderers
}

void gui::stopRenderers() {
	for (int i=0;i<9;++i) rend[i]->requestStop();
	for (int i=0;i<9;++i) {
		t[i].stop();
		do {} while (rend[i]->runs());
	}
}

void gui::startRenderers() {
	for (int i=0;i<9;++i) if ((i!=4) || (first)) {
		rend[i]->setRange(x0->value(),x1->value(),y0->value(),y1->value());
		rend[i]->setTree(tree[i]);							// Send the renderers their new trees
		rend[i]->setAlpha(alpha->isChecked());
		rend[i]->opMask()=opMask;
		rend[i]->start();								// Start them
		t[i].start(333);
	}
}

void gui::checkDone() {
	int j=0;
	for (int i=0;i<9;++i) if (!rend[i]->isRunning()) ++j;
}

void gui::x0Slot(int i) { if (x1->value()<=i) x1->setValue(i+1); first=true; }
void gui::x1Slot(int i) { if (x0->value()>=i) x0->setValue(i-1); first=true; }
void gui::y0Slot(int i) { if (y1->value()<=i) y1->setValue(i+1); first=true; }
void gui::y1Slot(int i) { if (y0->value()>=i) y0->setValue(i-1); first=true; }

void gui::renderWindowClosed(imagewindow *ptr,int w,int h) {
	if (ptr==lastRenderWindow) lastRenderWindow=NULL;
}

void gui::renderWindowSelect(treeNode *tn) {
	stopRenderers();
	delete tree[4];
	opMask=tn->getOpMask();
	displayOpMask();
	tree[4]=tn->copy();
	selectGenome(4,true);
}

void gui::t1Slot() {									// Timer slots to draw the images
	if (!rend[0]->isRunning()) t[0].stop();	
	g1->setPixmap(QPixmap::fromImage(*(rend[0]->image())));
}
void gui::t2Slot() {
	if (!rend[1]->isRunning()) t[1].stop();
	g2->setPixmap(QPixmap::fromImage(*(rend[1]->image())));
}
void gui::t3Slot() {
	if (!rend[2]->isRunning()) t[2].stop();
	g3->setPixmap(QPixmap::fromImage(*(rend[2]->image())));
}
void gui::t4Slot() {
	if (!rend[3]->isRunning()) t[3].stop();
	g4->setPixmap(QPixmap::fromImage(*(rend[3]->image())));
}
void gui::t5Slot() {
	if (!rend[4]->isRunning()) t[4].stop();
	g5->setPixmap(QPixmap::fromImage(*(rend[4]->image())));
}
void gui::t6Slot() {
	if (!rend[5]->isRunning()) t[5].stop();
	g6->setPixmap(QPixmap::fromImage(*(rend[5]->image())));
}
void gui::t7Slot() {
	if (!rend[6]->isRunning()) t[6].stop();
	g7->setPixmap(QPixmap::fromImage(*(rend[6]->image())));
}
void gui::t8Slot() {
	if (!rend[7]->isRunning()) t[7].stop();
	g8->setPixmap(QPixmap::fromImage(*(rend[7]->image())));
}
void gui::t9Slot() {
	if (!rend[8]->isRunning()) t[8].stop();
	g9->setPixmap(QPixmap::fromImage(*(rend[8]->image())));
}

void gui::saveConfig() {
	QSettings s(QSettings::UserScope,"subsymbolics.com","imagespace");
	s.setValue("/renderer/x0"		,x0		->value());
	s.setValue("/renderer/y0"		,y0		->value());
	s.setValue("/renderer/x1"		,x1		->value());
	s.setValue("/renderer/y1"		,y1		->value());
	s.setValue("/renderer/hres"		,hres		->value());
	s.setValue("/renderer/vres"		,vres		->value());
	s.setValue("/renderer/alpha"		,alpha		->isChecked());
	s.setValue("/renderer/autoopen"		,autoOpen	->isChecked());
	s.setValue("/tree/decay"		,decay		->value());
	s.setValue("/tree/maxsubnodes"		,maxSubnodes	->value());
	s.setValue("/tree/mutation"		,mutation	->value());
	unsigned long long i;
	i=opMask;
	s.setValue("/ops/ops1",(int)(i&0xffff));
	i=i >> 16;
	s.setValue("/ops/ops2",(int)(i&0xffff));
	i=i >> 16;	
	s.setValue("/ops/ops3",(int)(i&0xffff));
	i=i >> 16;
	s.setValue("/ops/ops4",(int)(i&0xffff));
}

void gui::loadConfig() {
	QSettings s(QSettings::UserScope,"subsymbolics.com","imagespace");
	x0		->setValue	(s.value("/renderer/x0"		,   -3).toInt ());
	y0		->setValue	(s.value("/renderer/y0"		,   -3).toInt ());
	x1		->setValue	(s.value("/renderer/x1"		,    3).toInt ());
	y1		->setValue	(s.value("/renderer/y1"		,    3).toInt ());
	hres		->setValue	(s.value("/renderer/hres"	,  256).toInt ());
	vres		->setValue	(s.value("/renderer/vres"	,  256).toInt ());
	alpha		->setChecked	(s.value("/renderer/alpha"	,false).toBool());
	autoOpen	->setChecked	(s.value("/renderer/autoopen"	, true).toBool());
	decay		->setValue	(s.value("/tree/decay"		,   20).toInt ());
	maxSubnodes	->setValue	(s.value("/tree/maxsubnodes"	,    2).toInt ());
	mutation	->setValue	(s.value("/tree/mutation"	,   10).toInt ());
	unsigned short i1,i2,i3,i4;
	i1=(unsigned short)s.value("/ops/ops1",0xffff).toInt();
	i2=(unsigned short)s.value("/ops/ops2",0xffff).toInt();
	i3=(unsigned short)s.value("/ops/ops3",0xffff).toInt();
	i4=(unsigned short)s.value("/ops/ops4",0xffff).toInt();
	opMask =i4; opMask=opMask<<16;
	opMask|=i3; opMask=opMask<<16;
	opMask|=i2; opMask=opMask<<16;
	opMask|=i1;
	displayOpMask();
}


void gui::displayOpMask() {
	op_tanh  ->setChecked(opMask&OP_TANH  );
	op_sin   ->setChecked(opMask&OP_SIN   );
	op_cos   ->setChecked(opMask&OP_COS   );
	op_gauss ->setChecked(opMask&OP_GAUSS );
	op_prod  ->setChecked(opMask&OP_MULT  );
	op_qprod ->setChecked(opMask&OP_QMULT );
	op_sum   ->setChecked(opMask&OP_ADD   );
	op_sh0   ->setChecked(opMask&OP_ADDS0 );
	op_sh1   ->setChecked(opMask&OP_ADDS1 );
	op_sh2   ->setChecked(opMask&OP_ADDS2 );
	op_sh3   ->setChecked(opMask&OP_ADDS3 );
	op_perm01->setChecked(opMask&OP_PERM01);
	op_perm02->setChecked(opMask&OP_PERM02);
	op_perm03->setChecked(opMask&OP_PERM03);
	op_perm12->setChecked(opMask&OP_PERM12);
	op_perm13->setChecked(opMask&OP_PERM13);
	op_perm23->setChecked(opMask&OP_PERM23);
}

void gui::tanhSlot  (bool b) { if (b) opMask|=OP_TANH;   else opMask&=~OP_TANH;   }
void gui::cosSlot   (bool b) { if (b) opMask|=OP_COS;    else opMask&=~OP_COS;    }
void gui::sinSlot   (bool b) { if (b) opMask|=OP_SIN;    else opMask&=~OP_SIN;    }
void gui::gaussSlot (bool b) { if (b) opMask|=OP_GAUSS;  else opMask&=~OP_GAUSS;  }
void gui::prodSlot  (bool b) { if (b) opMask|=OP_MULT;   else opMask&=~OP_MULT;   }
void gui::qprodSlot (bool b) { if (b) opMask|=OP_QMULT;  else opMask&=~OP_QMULT;  }
void gui::addSlot   (bool b) { if (b) opMask|=OP_ADD;    else opMask&=~OP_ADD;    }
void gui::sh0Slot   (bool b) { if (b) opMask|=OP_ADDS0;  else opMask&=~OP_ADDS0;  }
void gui::sh1Slot   (bool b) { if (b) opMask|=OP_ADDS1;  else opMask&=~OP_ADDS1;  }
void gui::sh2Slot   (bool b) { if (b) opMask|=OP_ADDS2;  else opMask&=~OP_ADDS2;  }
void gui::sh3Slot   (bool b) { if (b) opMask|=OP_ADDS3;  else opMask&=~OP_ADDS3;  }
void gui::perm01Slot(bool b) { if (b) opMask|=OP_PERM01; else opMask&=~OP_PERM01; }
void gui::perm02Slot(bool b) { if (b) opMask|=OP_PERM02; else opMask&=~OP_PERM02; }
void gui::perm03Slot(bool b) { if (b) opMask|=OP_PERM03; else opMask&=~OP_PERM03; }
void gui::perm12Slot(bool b) { if (b) opMask|=OP_PERM12; else opMask&=~OP_PERM12; }
void gui::perm13Slot(bool b) { if (b) opMask|=OP_PERM13; else opMask&=~OP_PERM13; }
void gui::perm23Slot(bool b) { if (b) opMask|=OP_PERM23; else opMask&=~OP_PERM23; }

void gui::saveImageSlot(QImage &im, QString format, int compression) {
	QString sfx="";
	if (format=="PNG")	sfx="*.png *.PNG"; else
				sfx="*.jpg *.JPG";
	QString s=QFileDialog::getSaveFileName(this,"Choose a filename","./",sfx);
	if (s!="") {
		QFileInfo fi(s);
		QDir d=fi.dir();
		QString s2=d.absolutePath()+"/"+fi.baseName();
		if (format=="PNG") s2+=".png"; else s2+=".jpg";
		cout << "SAVING " << s2.toAscii().constData() << endl;
		im.save(s2,format.toAscii().constData(),100-compression);
		cout << "COMPRESSION " << compression << endl;
	} 
}
