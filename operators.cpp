#include "operators.h"

treeNode::treeNode(bool empty, double prb, double dcy, int ord, double mprb) {
	if (!empty) mutate(prb,dcy,ord,mprb); else {
		prob =prb;
		decay=dcy;
		order=ord;
		mprob=mprb;
	}
}

treeNode::~treeNode() {
	for (unsigned int i=0;i<subnode.size();++i) delete subnode[i];
}

int treeNode::mutate(double prb, double dcy, int ord, double mprb) {
	prob =prb;
	decay=dcy;
	order=ord;
	mprob=mprb;
	int m=0;
	double probability_of_idNode=((double)random()/(double)RAND_MAX);
	if (probability_of_idNode>prob) {
		scalar=(((double)random()/(double)RAND_MAX)*2.0)-1.0;
		opType=OP_ID;
		++m;
	} else {
		double probability_of_mutation=((double)random()/(double)RAND_MAX);
		if ((probability_of_mutation<mprob) || (subnode.size()==0)) {
			++m;
			scalar=(((double)random()/(double)RAND_MAX)*2.0)-1.0;
			opType=rand();
			opType&=~OP_ID;
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
	unsigned int nrOfSubnodes=random()%(order+1);
	if (subnode.size()<nrOfSubnodes) do {
		subnode.insert(subnode.end(),new treeNode(false,prob*(1.0-decay),decay,order,mprob));
	} while (subnode.size()<nrOfSubnodes);

	if (subnode.size()>nrOfSubnodes) do {
		delete subnode[0];
		subnode.erase(subnode.begin());
	} while (subnode.size()>nrOfSubnodes);
	int m=0;
	for(unsigned i=0;i<subnode.size();++i) m+=subnode[i]->mutate(prob*(1.0-decay),decay,order,mprob);
	return(m);
}

D4 treeNode::get(double x, double y) {
	D4 v;

	if (opType & OP_ID) {
		v[0]=x+scalar; v[1]=y+scalar; v[2]=x-scalar; v[3]=y-scalar;
	} else {

		if (opType & OP_MULT) {
			v[0]=1.0; v[1]=1.0; v[2]=1.0; v[3]=1.0; for (unsigned int i=0;i<subnode.size();++i) v*= subnode[i]->get(x,y);
		} else
		if (opType & OP_ADD) {
			v[0]=0.0; v[1]=0.0; v[2]=0.0; v[3]=0.0; for (unsigned int i=0;i<subnode.size();++i) v+= subnode[i]->get(x,y);
			if (subnode.size()>0) {
				double q=1.0/(double)subnode.size();
				v[0]*=q;
				v[1]*=q;
				v[2]*=q;
				v[3]*=q;
			}
		} else
		if (opType & OP_QMULT) {
			v[0]=1.0; v[1]=0.0; v[2]=0.0; v[3]=0.0; for (unsigned int i=0;i<subnode.size();++i) v=v%subnode[i]->get(x,y);
		}
		if (opType & OP_ADDS0) v[0]+=scalar;
		if (opType & OP_ADDS1) v[1]+=scalar;
		if (opType & OP_ADDS2) v[2]+=scalar;
		if (opType & OP_ADDS3) v[3]+=scalar;
		if (opType & OP_PERM01) { double d=v[0]; v[0]=v[1]; v[1]=d; }
		if (opType & OP_PERM02) { double d=v[0]; v[0]=v[2]; v[2]=d; }
		if (opType & OP_PERM03) { double d=v[0]; v[0]=v[3]; v[3]=d; }
		if (opType & OP_PERM12) { double d=v[1]; v[1]=v[2]; v[2]=d; }
		if (opType & OP_PERM13) { double d=v[1]; v[1]=v[3]; v[3]=d; }
		if (opType & OP_PERM23) { double d=v[2]; v[2]=v[3]; v[3]=d; }

		if (opType & OP_TANH)   {
			for (unsigned int i=0;i<4;++i) v[i]=tanh(v[i]);
		} else
		if (opType & OP_GAUSS)	{
			for (unsigned int i=0;i<4;++i) v[i]=1.0/(exp(fabs(v[i])));
		} else
		if (opType & OP_SIN) {
			for (unsigned int i=0;i<4;++i) v[i]=sin(v[i]);
		} else
		if (opType & OP_COS) {
			for (unsigned int i=0;i<4;++i) v[i]=cos(v[i]);
		} else {
			for (unsigned int i=0;i<4;++i) v[i]=tanh(v[i]);
		}

	}
	return(v);
}

void treeNode::setNodeType(unsigned int nr) { opType=nr; }

unsigned int treeNode::getNodeType	() { return(opType); }

treeNode* treeNode::copy() {
	treeNode *t=new treeNode(true,prob,decay,order,mprob);
	t->scalar=scalar;
	t->opType=opType;
	t->subnode.clear();
	if (!(opType & OP_ID))
		for (unsigned int i=0;i<subnode.size();++i) t->subnode.insert(t->subnode.end(),subnode[i]->copy());
	return(t);
}

bool treeNode::save(const char *filename) {
	ofstream ofs(filename,ios::out);
	if (ofs) {
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
		load(ifs);
		ifs.close();
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
		subnode.insert(subnode.end(),new treeNode(true,0,0,0,0));
		subnode[i]->load(ifs);
	}
}

void treeNode::save(ofstream &ofs) {
	ofs << subnode.size() << " " << scalar << " " << opType << endl;
	for (unsigned int i=0;i<subnode.size();++i) subnode[i]->save(ofs);
}

