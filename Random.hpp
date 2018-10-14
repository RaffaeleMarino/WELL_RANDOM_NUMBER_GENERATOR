//
//  Random.hpp
//
//  Created by Raffaele Marino on 2018-08-25.
//  Copyright © 2018 Raffaele Marino. All rights reserved.
//

/*Class Random Using WELL Algorithm, minimal modifications for compatibility*/

/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#ifndef Random_h
#define Random_h
#include "Header.h"
#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define Identity(v) (v)

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;


#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000001fU]
#define VM2           STATE[(state_i+M2) & 0x0000001fU]
#define VM3           STATE[(state_i+M3) & 0x0000001fU]
#define VRm1          STATE[(state_i+31) & 0x0000001fU]
#define newV0         STATE[(state_i+31) & 0x0000001fU]
#define newV1         STATE[state_i                   ]


#define INITLEN 32


class Random{
    
    
public:
    
    /*declaration public members for the Random.hpp*/
    
    Random(){   /*constructor*/
        WellRandomInitialization();  /*initialization Well generator*/
    };
    
    ~Random(){}; /*destructor*/
    
    void print_seed(); /*print seed*/
    void print_seeds_on_file(); /*print  seed on file*/
    double random_number(); /*random number from a uniform distribution*/
    long unsigned int seed(); /*return seed*/
    
private:
    
    /*declaration private variables for the Random.hpp*/
    long unsigned int _seed; /*random number generator seed*/
    
    vector<unsigned int> _vecinittoprint; /*vector which stores the 32 seeds for initialization*/
    
    void InitWELLRNG1024a (unsigned int *init); /*initialization Well Random Generator*/
    
    double WELLRNG1024a (void); /*Random number generator*/
    
    unsigned int GetRandom(); /*return a seed*/
    
    void WellRandomInitialization();
    
    
};


inline double Random::random_number(){return WELLRNG1024a();}

inline void Random::print_seed(){cout<<_seed<<endl;}

inline unsigned int Random::seed(){return _seed;}

inline void Random::InitWELLRNG1024a (unsigned int *init){
    int j;
    state_i = 0;
    for (j = 0; j < R; j++)
        STATE[j] = init[j];
}

inline double Random::WELLRNG1024a (void){ /*random number generator*/
    z0    = VRm1;
    z1    = Identity(V0)       ^ MAT0POS (8, VM1);
    z2    = MAT0NEG (-19, VM2) ^ MAT0NEG(-14,VM3);
    newV1 = z1                 ^ z2;
    newV0 = MAT0NEG (-11,z0)   ^ MAT0NEG(-7,z1)    ^ MAT0NEG(-13,z2) ;
    state_i = (state_i + 31) & 0x0000001fU;
    return ((double) STATE[state_i]  * FACT);
}

inline unsigned int Random::GetRandom(){/*seed*/
    unsigned int ris;
    
    FILE *devran = fopen("/dev/urandom","r");
    
    fread(&ris, 4, 1, devran);
    
    fclose(devran);
    
    return ris;
}

inline void Random::WellRandomInitialization(){
    _seed=GetRandom();
    if (_seed < 1) {
        cout<<"ERROR in  RANDOM CLASS: SEED < 1"<<endl;
        exit(-1);
    }
    srandom(_seed);
    unsigned int init[INITLEN];
    double j=0;
    for (int i=0;i<INITLEN;++i) {
        init[i] = (unsigned int) random();
        _vecinittoprint.push_back(init[i]);
    }
    InitWELLRNG1024a(init);
    for (int i=0;i<20;++i) j = WELLRNG1024a();
}

void Random::print_seeds_on_file(){
    
    string mystringseedinit;
    ostringstream seedsite_str;
    ostringstream nsite_str;
    seedsite_str<<_seed;
    
    const string titleseed="Seed_init_N=";
    const string seedstring="seed=";
    const string dat=".txt";
    mystringseedinit=directory; //plese go to header_s.h and fill the path
    mystringseedinit+=titleseed;
    mystringseedinit+=nsite_str.str();
    mystringseedinit+=seedstring;
    mystringseedinit+=seedsite_str.str();
    mystringseedinit+=dat;
    
    ofstream outfilewriteseed(mystringseedinit.c_str(), ios_base::app );
    for (int ci=0; ci<INITLEN; ++ci) {
        outfilewriteseed<<_vecinittoprint[ci]<<endl;
    }
}

#endif /* Random_h */
