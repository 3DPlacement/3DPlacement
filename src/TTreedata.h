#ifndef TTreedata_H_
#define TTreedata_H_
#include "Basedata.h"
#include "TTree.h"
#include "Placement.h"

class TTree_data : public Base_data
{
public:
    TTree_data(double _pMove,double _pSwap,TTree* _tree);
    double getans(){ return 1.0 - NetV / tree->getPlacement().getVolume(); }
    void move();
    void undo(){ tree->undo(); }
    void update(){ minplacement=tree->getPlacement(); }
    Placement get_best_Placement(){ return minplacement; }

private:
    double pMove,pSwap;
    TTree* tree;
    Placement minplacement;
    double NetV;
};

#endif //TTreedata_H_ 
