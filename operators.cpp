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

void treeNode::mutate(double prb, double dcy, int ord, double mprb) {
	prob =prb;
	decay=dcy;
	order=ord;
	mprob=mprb;
	double probability_of_idNode=((double)random()/(double)RAND_MAX);
	if (probability_of_idNode>prob) {
		opType=OP_ID;
	} else {
		double probability_of_mutation=((double)random()/(double)RAND_MAX);
		if ((probability_of_mutation<mprob) || (subnode.size()==0)) {
			scalar=(double)random()/(double)RAND_MAX;
			opType=rand();
			opType&=~OP_ID;
			createSubnodes();
		}
	}
}

void treeNode::createSubnodes() {
	unsigned int nrOfSubnodes=random()%(order+1);
	if (subnode.size()<nrOfSubnodes) do {
		subnode.insert(subnode.end(),new treeNode(false,prob*(1.0-decay),decay,order,mprob));
	} while (subnode.size()<nrOfSubnodes);

	if (subnode.size()>nrOfSubnodes) do {
		delete subnode[0];
		subnode.erase(subnode.begin());
	} while (subnode.size()>nrOfSubnodes);
	for(unsigned i=0;i<subnode.size();++i) subnode[i]->mutate(prob*(1.0-decay),decay,order,mprob);
}

D4 treeNode::get(double x, double y) {
	D4 v;

	if (opType & OP_ID) {
		v[0]=x; v[1]=y; v[2]=x; v[3]=y;
	} else {
		if (opType & OP_MULT) {
			v[0]=1.0; v[1]=1.0; v[2]=1.0; v[3]=1.0; for (unsigned int i=0;i<subnode.size();++i) v*= subnode[i]->get(x,y);
		} else
		if (opType & OP_ADD) {
			v[0]=0.0; v[1]=0.0; v[2]=0.0; v[3]=0.0; for (unsigned int i=0;i<subnode.size();++i) v+= subnode[i]->get(x,y);
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
