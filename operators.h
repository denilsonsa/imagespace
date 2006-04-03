// ---------------------------------------------------------------------------------
// SubSymbolics 2003
// ---------------------------------------------------------------------------------

#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdlib.h>											// ANSI C std. functions
#include <stdio.h>											// ANSI C i/o functions
#include <math.h>											// FP Math
#include "stlall.h"											// STL

#define MAXDEPTH 20

using namespace std;											// Standard namespace

#ifndef PI												// Define PI if necessary - taken from www.joyofpi.com
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502
#endif													// This should be enough even for a CRAY

class D4 {

	public:
	// CONSTRUCTOR (0,0,0,0)
	D4			(double d0=0, double d1=0, double d2=0, double d3=0) { d[0]=d0; d[1]=d1; d[2]=d2; d[3]=d3; }

	// UNARY OPERATORS
	double	norm		()	{ double r=0; for (int i=0;i<4;++i) r+=d[i]*d[i]; return(sqrt(r)); }

	// VECTOR/VECTOR
	D4	operator *	(D4 v)  { return(D4(d[0]*v[0],d[1]*v[1],d[2]*v[2],d[3]*v[3])); }
	D4	operator +	(D4 v)  { return(D4(d[0]+v[0],d[1]+v[1],d[2]+v[2],d[3]+v[3])); }
	D4	operator -	(D4 v)  { return(D4(d[0]-v[0],d[1]-v[1],d[2]-v[2],d[3]-v[3])); }
	D4	operator /	(D4 v)  { return(D4(d[0]/v[0],d[1]/v[1],d[2]/v[2],d[3]/v[3])); }
	D4	operator %	(D4 v)  { return(D4(d[0]*v[0]-d[1]*v[1]-d[2]*v[2]-d[3]*v[3],
						    d[0]*v[1]+d[1]*v[0]+d[2]*v[3]-d[3]*v[2],
						    d[0]*v[2]-d[1]*v[3]+d[2]*v[0]+d[3]*v[1],
						    d[0]*v[3]+d[1]*v[2]-d[2]*v[1]+d[3]*v[0])); }
	D4	sinus		()	{ return(D4(sin(d[0]),sin(d[1]),sin(d[2]),sin(d[3]))); }
	D4	tanhyp		()	{ return(D4(tanh(d[0]),tanh(d[1]),tanh(d[2]),tanh(d[3]))); }
	void	operator *=	(D4 v)  { for (int i=0;i<4;++i) d[i]*=v[i]; }
	void	operator +=	(D4 v)  { for (int i=0;i<4;++i) d[i]+=v[i]; }
	void	operator -=	(D4 v)  { for (int i=0;i<4;++i) d[i]-=v[i]; }
	void	operator /=	(D4 v)  { for (int i=0;i<4;++i) d[i]/=v[i]; }
	double	operator &	(D4 v)  { double r=0; for (int i=0;i<4;++i) r+=d[i]*v[i]; return(r); }

	// SCALAR
	void	operator *=	(double v) { for (int i=0;i<4;++i) d[i]*=v; }
	void	operator +=	(double v) { for (int i=0;i<4;++i) d[i]+=v; }
	void	operator -=	(double v) { for (int i=0;i<4;++i) d[i]-=v; }
	void	operator /=	(double v) { for (int i=0;i<4;++i) d[i]/=v; }


	// ACCESS
	double&	operator[]	(int i) { return(d[i]); }
	void	dump		()	{ cout << "(" << d[0] << "|" << d[1] << "|" << d[2] << "|" << d[3] << ")" << endl; }

	private:
		double d[4];
};

#define OP_ID			   1
#define OP_MULT			   2
#define OP_QMULT		   4
#define OP_ADD			   8

#define OP_ADDS0		  16
#define OP_ADDS1		  32
#define OP_ADDS2		  64
#define OP_ADDS3		 128

#define OP_TANH			 256
#define OP_COS			 512
#define OP_SIN			1024
#define OP_GAUSS		2048

#define OP_PERM01		4096
#define OP_PERM02		8192
#define OP_PERM03		16384
#define OP_PERM12		32768
#define OP_PERM13		65536
#define OP_PERM23		131072

#define OP_PADD01		OP_PERM23*2
#define OP_PADD02		OP_PADD01*2
#define OP_PADD03		OP_PADD02*2
#define OP_PADD10		OP_PADD03*2
#define OP_PADD12		OP_PADD10*2
#define OP_PADD13		OP_PADD12*2
#define OP_PADD20		OP_PADD13*2
#define OP_PADD21		OP_PADD20*2
#define OP_PADD23		OP_PADD21*2
#define OP_PADD30		OP_PADD23*2
#define OP_PADD31		OP_PADD30*2
#define OP_PADD32		OP_PADD31*2

#define ALLOP OP_ID | OP_TANH | OP_COS | OP_SIN | OP_GAUSS

/*#ifndef ZERO_MAX
#define ZERO_MAX 4
#endif*/

class treeNode {												// The treeNode
	public:
					treeNode	(bool empty=false, double prb=1.0, double dcy=0.5, 	// Construct a treeNode having subnodes with probability prb
			  				 int ord=2, double mprb=0, int dpth=0);			// Subsodes have subnodes with probability prb*dcy (Decay dcy should be in ]0;1[)
			       	       ~treeNode	();							// Destructor

		treeNode*		copy		();							// Returns a deep copy of this tree
		void			clear		();							// Delete all subnodes
		int			mutate		(double,double,int,double,int dpth=0);			// Mutate a node and it's subnodes (also, new values for prb, dcy, ord, mprb are transferred) returns number of mutations in tree
		D4			get		(double,double);					// Get RGBA vector at (x,y)
		void			setNodeType	(unsigned long long);					// Set a treenode's operation type
		unsigned long long	getNodeType	();							// Get a treenode's operation type
		void			setOpMask	(unsigned long long);					// Allow or forbid certain operations
		unsigned long long	getOpMask	();
		bool			save		(const char*);						// Save to file
		bool			load		(const char*);						// Load from file


	private:
		int			createSubnodes	();							// Creates subnodes if not already done so
		void			save		(ofstream&);						// Save to stream
		void			load		(ifstream&);						// Load from stream

		unsigned long long opType, ot, opmask;								// opType, defining the  node's function
		vector<treeNode*> subnode;									// If opType!=OP_ID, the treeNode will have subnodes
		double	 scalar, 										// A scalar value which is multiplied with the vector
			 prob, 											// The probability of this treeNode to have subnodes
			 decay,											// The subnode probability decay
			 mprob;											// The mutation probability of a subnode
		int	 order,											// The maximum number of subnodes this node can have
			 depth;											// The distance from this node to the root node - used to cut trees at MAXDEPTH
		//double	 zero_x[ZERO_MAX],zero_y[ZERO_MAX];
};

#endif
