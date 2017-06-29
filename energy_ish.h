#ifndef ENERGY_ISH_H
#define ENERGY_ISH_H

//#include <assert.h>
#include "graph.h"
#include <iostream>

class Energy_Ish : public Graph<short,short,int>
{
public:
	Energy_Ish(int dimx, int dimy, int dimg);
	~Energy_Ish();
	
	void Set_DataCost2(int i, short* vec);
	void Set_DataCost(int i, short* vec);
	void Set_DataCost(int i, int k, short val);
	
	void Set_UniformL1(short lam);
	void Set_L1_Reg(int i, int j,short lam);
	
	int maxflow(){
		return (Graph<short,short,int>::maxflow() + delta);
	};
	
private:
	int dimx,size,diml, delta;
};

/// Constructor.
/// For efficiency, it is advised to give appropriate hint sizes.
inline Energy_Ish::Energy_Ish(int dimx, int dimy, int dimg) : Graph<short,short,int>(dimx*dimy*(dimg-1),6*dimx*dimy*(dimg-1)), dimx(dimx), size(dimx*dimy), diml(dimg-1), delta(0)
{
	// Add Nodes and Vertical links (Data and Constraints), Data to zero
	for(int i=0; i<size; i++){
		this->add_node();
		for(int k=1; k<diml; k++){
			this->add_node();
			this->add_edge(i*diml+k-1, i*diml+k, 0, std::numeric_limits<short>::max()/2);
		}
	}
	
	//// Add Nodes and Vertical links (Data and Constraints), Data to zero
	//for(int i=0; i<size; i++) this->add_node();
	//
	//for(int k=1; k<diml; k){
	//	for(int i=0; i<size; i++){
	//		this->add_node();
	//		this->add_edge(i+(k-1)*size, i+k*size, 0, std::numeric_limits<short>::max()/2);
	//	}
	//}
}

/// Destructor
inline Energy_Ish::~Energy_Ish() {}

inline void Energy_Ish::Set_DataCost(int i, short *vec){
	// set min data energy to zero
	// garanties non negative data term
	short mmin=vec[0];
	for(int k=1; k<diml; k++) if( vec[k] < mmin) mmin=vec[k];
	delta += mmin;
	
	this->add_tweights(i*diml,vec[0]-mmin,0);
	for(int k=1; k<diml; k++) Set_Arcs_Cap(i*(diml-1)*2+2*(k-1),vec[k]-mmin);
	this->add_tweights(i*diml+diml-1,0,vec[diml]-mmin);
}

inline void Energy_Ish::Set_DataCost2(int i, short *vec){
	delta += vec[diml];
	for(int k=0; k<diml; k++) this->add_tweights(i*diml+k,vec[k]-vec[k+1],0);
}

inline void Energy_Ish::Set_DataCost(int i, int k, short val){
	if(k==0) this->add_tweights(i*diml,val,0);
	else if(k<diml) Set_Arcs_Cap(i*(diml-1)*2+2*(k-1),val);
	else this->add_tweights(i*diml+k-1,0,val);
}

inline void Energy_Ish::Set_UniformL1(short lam){
	for(int i=0; i<size; i++){
		if( i/dimx != 0) for(int k=0; k<diml; k++) this->add_edge((i-dimx)*diml+k,i*diml+k,lam,lam);
		if( i%dimx != 0) for(int k=0; k<diml; k++) this->add_edge((i-1)*diml+k,i*diml+k,lam,lam);
	}
}

inline void Energy_Ish::Set_L1_Reg(int i, int j,short lam){
	for(int k=0; k<diml; k++) this->add_edge(i*diml+k,j*diml+k,lam,lam);
}


#endif
