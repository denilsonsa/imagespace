#include "operators.h"

#include <stdlib.h>

treeNode::treeNode(bool empty, double prb, double dcy, int ord, double mprb, int dpth) {
	if (!empty) mutate(prb,dcy,ord,mprb,dpth); else {
		prob =prb;
		decay=dcy;
		order=ord;
		mprob=mprb;
	}
	//for (int i=0;i<ZERO_MAX;++i) { zero_x[i]= 5.0*(double)random()/(double)RAND_MAX; zero_y[i]=5.0*(double)random()/(double)RAND_MAX; }
	opmask=~0;
}

treeNode::~treeNode() {
	for (unsigned int i=0;i<subnode.size();++i) delete subnode[i];
}

int treeNode::mutate(double prb, double dcy, int ord, double mprb, int dpth) {
	prob =prb;
	decay=dcy;
	order=ord;
	mprob=mprb;
	depth=dpth;
	int m=0;
	double probability_of_idNode=((double)rand()/(double)RAND_MAX);
	if ((probability_of_idNode>prob) || (depth>MAXDEPTH)) {
		scalar=(((double)rand()/(double)RAND_MAX)*2.0)-1.0;
		opType=OP_ID;
		++m;
	} else {
		double probability_of_mutation=((double)rand()/(double)RAND_MAX);
		if ((probability_of_mutation<mprob) || (subnode.size()==0)) {
			/*for (int i=0;i<ZERO_MAX;++i) {
				zero_x[i]=5.0*(double)random()/(double)RAND_MAX; zero_y[i]=5.0*(double)random()/(double)RAND_MAX; 
			} */
			scalar=(((double)rand()/(double)RAND_MAX)*2.0)-1.0;
			opType=rand();
			opType*=rand();
			opType&=~ALLOP;
			++m;
			switch(rand()%4) {
				case 0: opType|=OP_TANH;  break;
				case 1: opType|=OP_GAUSS; break;
				case 2: opType|=OP_SIN;   break;
				case 3: opType|=OP_COS;   break;
			}
			m+=createSubnodes();
		}
	}
	return(m);
}

void treeNode::clear() {
	for (unsigned int i=0;i<subnode.size();++i) delete subnode[i];
	subnode.clear();
}

int treeNode::createSubnodes() {
	unsigned int nrOfSubnodes=(rand()%(order-1))+2;
	if (subnode.size()<nrOfSubnodes) do {
		subnode.insert(subnode.end(),new treeNode(false,prob*(1.0-decay),decay,order,mprob,++depth));
	} while (subnode.size()<nrOfSubnodes);

	if (subnode.size()>nrOfSubnodes) do {
		delete subnode[0];
		subnode.erase(subnode.begin());
	} while (subnode.size()>nrOfSubnodes);
	int m=0;
	for (unsigned i=0;i<subnode.size();++i) m+=subnode[i]->mutate(prob*(1.0-decay),decay,order,mprob,++depth);
	return(m);
}

unsigned long long treeNode::getOpMask() { return(opmask); }

void treeNode::setOpMask(unsigned long long opMask) {
	ot=opType & opMask;
	opmask=opMask;
	for (unsigned i=0;i<subnode.size();++i) subnode[i]->setOpMask(opMask);
}

D4 treeNode::get(double x, double y) {
	D4 v;
	if (ot & OP_ID) {
		/*for (int i=0;i<4;++i) {
			double	
				x1=fabs(x-zero_x[i]), y1=fabs(y-zero_y[i]),
				x2=fabs(x-zero_x[i]+5.0), y2=fabs(y-zero_y[i]+5.0),
				x3=fabs(x-zero_x[i]-5.0), y3=fabs(y-zero_y[i]-5.0);
			if (x2<x1) x1=x2;
			if (y2<y1) y1=y2;
			if (x3<x1) x1=x3;
			if (y3<y1) y1=y3;
			
			double n=8.0;
			double d=pow((pow(x1,n)+pow(y1,n)),1.0/n);
			v[i]=d;
		}*/
		
		v[0]=x; v[1]=y; v[2]=-x; v[3]=-y;
	} else {

		if (ot & OP_MULT) {
			v[0]=1.0; v[1]=1.0; v[2]=1.0; v[3]=1.0; for (unsigned int i=0;i<subnode.size();++i) v*= subnode[i]->get(x,y);
		} else
		if (ot & OP_QMULT) {
			v[0]=1.0; v[1]=0.0; v[2]=0.0; v[3]=0.0; for (unsigned int i=0;i<subnode.size();++i) v=v%subnode[i]->get(x,y);
		} else {
			v[0]=0.0; v[1]=0.0; v[2]=0.0; v[3]=0.0; for (unsigned int i=0;i<subnode.size();++i) v+=subnode[i]->get(x,y);
			
			if (subnode.size()>0) {
				double q=1.0/(double)subnode.size();
				v[0]*=q;
				v[1]*=q;
				v[2]*=q;
				v[3]*=q;
			}
		}

		if (ot & OP_ADDS0) v[0]+=scalar;
		if (ot & OP_ADDS1) v[1]+=scalar;
		if (ot & OP_ADDS2) v[2]+=scalar;
		if (ot & OP_ADDS3) v[3]+=scalar;

		if (ot & OP_PERM01) { double d=v[0]; v[0]=v[1]; v[1]=d; }
		if (ot & OP_PERM02) { double d=v[0]; v[0]=v[2]; v[2]=d; }
		if (ot & OP_PERM03) { double d=v[0]; v[0]=v[3]; v[3]=d; }
		if (ot & OP_PERM12) { double d=v[1]; v[1]=v[2]; v[2]=d; }
		if (ot & OP_PERM13) { double d=v[1]; v[1]=v[3]; v[3]=d; }
		if (ot & OP_PERM23) { double d=v[2]; v[2]=v[3]; v[3]=d; }

		if (opType & OP_PADD01) v[0]+=v[1];
		if (opType & OP_PADD02) v[0]+=v[2];
		if (opType & OP_PADD03) v[0]+=v[3];
		if (opType & OP_PADD10) v[1]+=v[0];
		if (opType & OP_PADD12) v[1]+=v[2];
		if (opType & OP_PADD13) v[1]+=v[3];
		if (opType & OP_PADD20) v[2]+=v[0];
		if (opType & OP_PADD21) v[2]+=v[1];
		if (opType & OP_PADD23) v[2]+=v[3];
		if (opType & OP_PADD30) v[3]+=v[0];
		if (opType & OP_PADD31) v[3]+=v[1];
		if (opType & OP_PADD32) v[3]+=v[2];

		for (int i=0;i<4;++i) v[i]*=scalar;

		if (ot & OP_TANH) {
			for (unsigned int i=0;i<4;++i) v[i]=tanh(v[i]);
		} else
		if (ot & OP_GAUSS) {
			for (unsigned int i=0;i<4;++i) v[i]=2.0/(exp(fabs(v[i])))-1.0;
		} else
		if (ot & OP_SIN) {
			for (unsigned int i=0;i<4;++i) v[i]=sin(v[i]);
		} else
		if (ot & OP_COS) {
			for (unsigned int i=0;i<4;++i) v[i]=cos(v[i]);
		} else {
			for (unsigned int i=0;i<4;++i) v[i]=sin(v[i]);
		}
  

	}      
	return(v);
}

void treeNode::setNodeType(unsigned long long nr) { opType=nr; }

unsigned long long treeNode::getNodeType  () { return(opType); }

treeNode* treeNode::copy() {
	treeNode *t=new treeNode(true,prob,decay,order,mprob,depth);
	t->scalar=scalar;
	t->opType=opType;
//	for (int i=0;i<ZERO_MAX;++i) { t->zero_x[i]=zero_x[i]; t->zero_y[i]=zero_y[i]; }
	t->subnode.clear();
	if (!(opType & OP_ID))
		for (unsigned int i=0;i<subnode.size();++i) t->subnode.insert(t->subnode.end(),subnode[i]->copy());
	return(t);
}

bool treeNode::save(const char *filename) {
	ofstream ofs(filename,ios::out);
	if (ofs) {
		ofs << opmask << endl;
		save(ofs);
		ofs.close();
		return(true);
	}
	return(false);
}

bool treeNode::load(const char *filename) {
	ifstream ifs(filename,ios::in);
	if (ifs) {
		clear();
		ifs >> opmask;
		load(ifs);
		ifs.close();
		setOpMask(opmask);
		return(true);
	}
	return(false);
}

void treeNode::load(ifstream &ifs) {
	int subnodes;
	ifs >> subnodes;
	ifs >> scalar;
	ifs >> opType;
	for (unsigned int i=0;i<subnodes;++i) {
		subnode.insert(subnode.end(),new treeNode(true,0,0,0,0,0));
		subnode[i]->load(ifs);
	}
}

void treeNode::save(ofstream &ofs) {
	ofs << subnode.size() << " " << scalar << " " << opType << endl;
	for (unsigned int i=0;i<subnode.size();++i) subnode[i]->save(ofs);
}

