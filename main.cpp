#include "control.h"									// Control object header

using namespace std;									// Use standard namespace

int main(int argc, char **argv) {
	srandom(time(NULL));
	QApplication app(argc,argv);
	control *w=new control(0);
	app.setMainWidget(w);
	w->show();
	return app.exec();
	
};
