#include "control.h"

control::control(QWidget *parent, const char *name, WFlags fl):MainWindow(parent,name,fl) {

	lastRenderWindow=NULL;

	for (int i=0;i<9;++i) {
		rend[i]=new renderer(this,0);
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

	connect(g1,SIGNAL(clicked()),this,SLOT(g1Slot()));
	connect(g2,SIGNAL(clicked()),this,SLOT(g2Slot()));
	connect(g3,SIGNAL(clicked()),this,SLOT(g3Slot()));
	connect(g4,SIGNAL(clicked()),this,SLOT(g4Slot()));
	connect(g5,SIGNAL(clicked()),this,SLOT(g5Slot()));
	connect(g6,SIGNAL(clicked()),this,SLOT(g6Slot()));
	connect(g7,SIGNAL(clicked()),this,SLOT(g7Slot()));
	connect(g8,SIGNAL(clicked()),this,SLOT(g8Slot()));
	connect(g9,SIGNAL(clicked()),this,SLOT(g9Slot()));

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
	first=true;
	startRenderers();
}

control::~control() {
	for (int i=0;i<9;++i) {
		delete rend[i];
		delete tree[i];
	}
}

void control::renderSlot() {
	if ((autoOpen->isChecked()) || (lastRenderWindow==NULL)) {
		lastRenderWindow=new renderwindow(0);
		connect(lastRenderWindow,SIGNAL(closed(renderwindow*)),this,SLOT(renderWindowClosed(renderwindow*)));
	}
	lastRenderWindow->copyTree(selectedTree);
	lastRenderWindow->setRange(x0->value(),x1->value(),y0->value(),y1->value());
	lastRenderWindow->setResolution(hres->value(),vres->value());
	lastRenderWindow->show();
	lastRenderWindow->start();
}

void control::fileSlot(int nr) {
	QString s;
	switch(nr) {
		case 0: s=QFileDialog::getOpenFileName("", "",this, "open file dialog","Choose a file" );
			break;
		case 1: s=QFileDialog::getSaveFileName("", "",this, "save file dialog","Choose a file" );
			break;
		case 2: close();
			break;
	}
}

void control::closeEvent (QCloseEvent *e)  {
	e->accept();
}

void control::g1Slot() { selectGenome(0); }							// Button click slots (genome selected)
void control::g2Slot() { selectGenome(1); }
void control::g3Slot() { selectGenome(2); }
void control::g4Slot() { selectGenome(3); }
void control::g5Slot() { selectGenome(4); }
void control::g6Slot() { selectGenome(5); }
void control::g7Slot() { selectGenome(6); }
void control::g8Slot() { selectGenome(7); }
void control::g9Slot() { selectGenome(8); }

void control::rend1DoneSlot () { checkDone(); }
void control::rend2DoneSlot () { checkDone(); }
void control::rend3DoneSlot () { checkDone(); }
void control::rend4DoneSlot () { checkDone(); }
void control::rend5DoneSlot () { checkDone(); }
void control::rend6DoneSlot () { checkDone(); }
void control::rend7DoneSlot () { checkDone(); }
void control::rend8DoneSlot () { checkDone(); }
void control::rend9DoneSlot () { checkDone(); }

void control::selectGenome(int nr) {
	stopRenderers();									// Stop all renderers
	selectedTree=tree[nr];
	if (nr!=4) g5->setPaletteBackgroundPixmap(*(rend[nr]->image()));			// Copy the selected pixmap to the central button
	for (int i=0;i<9;++i) if (i!=nr) delete tree[i];					// Delete unselected trees
	for (int i=0;i<9;++i) if (i!=nr) tree[i]=tree[nr]->copy();				// Clone the selected tree nine times
	for (int i=0;i<9;++i) if (i!=4) {							// Mutate all trees with the exception of the central tree
		do { } while (tree[i]->mutate(1.0,(double)decay->value()/100.0,maxSubnodes->value(),(double)mutation->value()/100.0)==0);
	}
	selectedTree=tree[4];
	startRenderers();									// Start all renderers
	first=false;
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
		rend[i]->start();								// Start them
		t[i].start(333);
	}
}

void control::checkDone() {
	int j=0;
	for (int i=0;i<9;++i) if (!rend[i]->running()) ++j;
	if (j==8) {
		/*g1->setPaletteBackgroundPixmap(*(rend[0]->image()));
		g2->setPaletteBackgroundPixmap(*(rend[1]->image()));
		g3->setPaletteBackgroundPixmap(*(rend[2]->image()));
		g4->setPaletteBackgroundPixmap(*(rend[3]->image()));
		g6->setPaletteBackgroundPixmap(*(rend[5]->image()));
		g7->setPaletteBackgroundPixmap(*(rend[6]->image()));
		g8->setPaletteBackgroundPixmap(*(rend[7]->image()));
		g9->setPaletteBackgroundPixmap(*(rend[8]->image()));*/
	}
}

void control::x0Slot(int i) { if (x1->value()<=i) x1->setValue(i+1); first=true; }
void control::x1Slot(int i) { if (x0->value()>=i) x0->setValue(i-1); first=true; }
void control::y0Slot(int i) { if (y1->value()<=i) y1->setValue(i+1); first=true; }
void control::y1Slot(int i) { if (y0->value()>=i) y0->setValue(i-1); first=true; }

void control::renderWindowClosed(renderwindow *ptr) {
	if (ptr==lastRenderWindow) lastRenderWindow=NULL;
}

void control::t1Slot() {
	if (!rend[0]->running()) t[0].stop();
	g1->setPaletteBackgroundPixmap(*(rend[0]->image()));
}
void control::t2Slot() {
	if (!rend[1]->running()) t[1].stop();
	g2->setPaletteBackgroundPixmap(*(rend[1]->image()));
}
void control::t3Slot() {
	if (!rend[2]->running()) t[2].stop();
	g3->setPaletteBackgroundPixmap(*(rend[2]->image()));
}
void control::t4Slot() {
	if (!rend[3]->running()) t[3].stop();
	g4->setPaletteBackgroundPixmap(*(rend[3]->image()));
}
void control::t5Slot() {
	if (!rend[4]->running()) t[4].stop();
	g5->setPaletteBackgroundPixmap(*(rend[4]->image()));
}
void control::t6Slot() {
	if (!rend[5]->running()) t[5].stop();
	g6->setPaletteBackgroundPixmap(*(rend[5]->image()));
}
void control::t7Slot() {
	if (!rend[6]->running()) t[6].stop();
	g7->setPaletteBackgroundPixmap(*(rend[6]->image()));
}
void control::t8Slot() {
	if (!rend[7]->running()) t[7].stop();
	g8->setPaletteBackgroundPixmap(*(rend[7]->image()));
}
void control::t9Slot() {
	if (!rend[8]->running()) t[8].stop();
	g9->setPaletteBackgroundPixmap(*(rend[8]->image()));
}


