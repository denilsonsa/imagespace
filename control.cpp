#include "control.h"

control::control(QWidget *parent, const char *name, WFlags fl):MainWindow(parent,name,fl) {
	QImage bg=QImage(qembed_findData("alpha.png"));
	
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

	connect(g1,SIGNAL(clicked()),this,SLOT(g1Slot())); g1->setPaletteBackgroundPixmap(bg);
	connect(g2,SIGNAL(clicked()),this,SLOT(g2Slot())); g2->setPaletteBackgroundPixmap(bg);
	connect(g3,SIGNAL(clicked()),this,SLOT(g3Slot())); g3->setPaletteBackgroundPixmap(bg);
	connect(g4,SIGNAL(clicked()),this,SLOT(g4Slot())); g4->setPaletteBackgroundPixmap(bg);
	connect(g5,SIGNAL(clicked()),this,SLOT(g5Slot())); g5->setPaletteBackgroundPixmap(bg);
	connect(g6,SIGNAL(clicked()),this,SLOT(g6Slot())); g6->setPaletteBackgroundPixmap(bg);
	connect(g7,SIGNAL(clicked()),this,SLOT(g7Slot())); g7->setPaletteBackgroundPixmap(bg);
	connect(g8,SIGNAL(clicked()),this,SLOT(g8Slot())); g8->setPaletteBackgroundPixmap(bg);
	connect(g9,SIGNAL(clicked()),this,SLOT(g9Slot())); g9->setPaletteBackgroundPixmap(bg);

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
	startRenderers();
}

control::~control() {
	for (int i=0;i<9;++i) {
		delete rend[i];
		delete tree[i];
	}
}

void control::alphaSlot(bool a) { first=true; }

void control::renderSlot() {
	if ((autoOpen->isChecked()) || (lastRenderWindow==NULL)) {
		lastRenderWindow=new renderwindow(0);
		connect(lastRenderWindow,SIGNAL(closed(renderwindow*)),this,SLOT(renderWindowClosed(renderwindow*)));
	}
	lastRenderWindow->copyTree(selectedTree);
	lastRenderWindow->setRange(x0->value(),x1->value(),y0->value(),y1->value());
	lastRenderWindow->setResolution(hres->value(),vres->value());
	lastRenderWindow->show();
	lastRenderWindow->setAlpha(alpha->isChecked());
	lastRenderWindow->start();
}

void control::saveSlot() {
	QString s=QFileDialog::getSaveFileName("", "",this, "save file dialog","Choose a filename" );
	if (s!="") {
		stopRenderers();
		tree[4]->save(s.ascii());
	}
}
void control::loadSlot() {
	QString s=QFileDialog::getOpenFileName("", "",this, "open file dialog","Choose a file" );
	if (s!="") {
		stopRenderers();
		tree[4]->load(s.ascii());
		selectGenome(4,true);
	}
}

void control::closeEvent (QCloseEvent *e)  {
	e->accept();
}

void control::g1Slot() { selectGenome(0,first); }						// Button click slots (genome selected)
void control::g2Slot() { selectGenome(1,first); }
void control::g3Slot() { selectGenome(2,first); }
void control::g4Slot() { selectGenome(3,first); }
void control::g5Slot() { selectGenome(4,first); }
void control::g6Slot() { selectGenome(5,first); }
void control::g7Slot() { selectGenome(6,first); }
void control::g8Slot() { selectGenome(7,first); }
void control::g9Slot() { selectGenome(8,first); }

void control::rend1DoneSlot () { checkDone(); }
void control::rend2DoneSlot () { checkDone(); }
void control::rend3DoneSlot () { checkDone(); }
void control::rend4DoneSlot () { checkDone(); }
void control::rend5DoneSlot () { checkDone(); }
void control::rend6DoneSlot () { checkDone(); }
void control::rend7DoneSlot () { checkDone(); }
void control::rend8DoneSlot () { checkDone(); }
void control::rend9DoneSlot () { checkDone(); }

void control::selectGenome(int nr, bool frst) {							// Select a new genome
	first=frst;
	stopRenderers();									// Stop all renderers
	selectedTree=tree[nr];
	if (nr!=4) g5->setPixmap(*(rend[nr]->image()));						// Copy the selected pixmap to the central button
	for (int i=0;i<9;++i) if (i!=nr) delete tree[i];					// Delete unselected trees
	for (int i=0;i<9;++i) if (i!=nr) tree[i]=tree[nr]->copy();				// Clone the selected tree nine times
	for (int i=0;i<9;++i) if (i!=4) {							// Mutate all trees with the exception of the central tree
		do { } while (tree[i]->mutate(1.0,(double)decay->value()/100.0,maxSubnodes->value(),(double)mutation->value()/100.0)==0);
	}
	selectedTree=tree[4];
	startRenderers();									// Start all renderers
}

void control::stopRenderers() {
	for (int i=0;i<9;++i) rend[i]->requestStop();
	for (int i=0;i<9;++i) {
		t[i].stop();
		do {} while (rend[i]->runs());
	}
}

void control::startRenderers() {
	for (int i=0;i<9;++i) if ((i!=4) || (first)) {
		rend[i]->setRange(x0->value(),x1->value(),y0->value(),y1->value());
		rend[i]->setTree(tree[i]);							// Send the renderers their new trees
		rend[i]->setAlpha(alpha->isChecked());
		rend[i]->start();								// Start them
		t[i].start(333);
	}
}

void control::checkDone() {
	int j=0;
	for (int i=0;i<9;++i) if (!rend[i]->running()) ++j;
}

void control::x0Slot(int i) { if (x1->value()<=i) x1->setValue(i+1); first=true; }
void control::x1Slot(int i) { if (x0->value()>=i) x0->setValue(i-1); first=true; }
void control::y0Slot(int i) { if (y1->value()<=i) y1->setValue(i+1); first=true; }
void control::y1Slot(int i) { if (y0->value()>=i) y0->setValue(i-1); first=true; }

void control::renderWindowClosed(renderwindow *ptr) {
	if (ptr==lastRenderWindow) lastRenderWindow=NULL;
}

void control::t1Slot() {									// Timer slots to draw the images
	if (!rend[0]->running()) t[0].stop();	
	g1->setPixmap(*(rend[0]->image()));
}
void control::t2Slot() {
	if (!rend[1]->running()) t[1].stop();
	g2->setPixmap(*(rend[1]->image()));
}
void control::t3Slot() {
	if (!rend[2]->running()) t[2].stop();
	g3->setPixmap(*(rend[2]->image()));
}
void control::t4Slot() {
	if (!rend[3]->running()) t[3].stop();
	g4->setPixmap(*(rend[3]->image()));
}
void control::t5Slot() {
	if (!rend[4]->running()) t[4].stop();
	g5->setPixmap(*(rend[4]->image()));
}
void control::t6Slot() {
	if (!rend[5]->running()) t[5].stop();
	g6->setPixmap(*(rend[5]->image()));
}
void control::t7Slot() {
	if (!rend[6]->running()) t[6].stop();
	g7->setPixmap(*(rend[6]->image()));
}
void control::t8Slot() {
	if (!rend[7]->running()) t[7].stop();
	g8->setPixmap(*(rend[7]->image()));
}
void control::t9Slot() {
	if (!rend[8]->running()) t[8].stop();
	g9->setPixmap(*(rend[8]->image()));
}

