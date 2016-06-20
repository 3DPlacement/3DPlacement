#include "TTreedata.h"
#include "Random.h"

TTree_data::TTree_data(double _pMove,double _pSwap,TTree* _tree)
                : pMove(_pMove), pSwap(_pSwap), tree(_tree)
{

    assert(pMove >= 0);
    assert(pSwap >= 0);
    assert(1 - pMove - pSwap >= 0);
    
    minplacement= tree->getPlacement();
    NetV=minplacement.getNetVolume();
}

void TTree_data::move()
{
     double method(Random::getInstance().getRandomDouble(0, 1));
     if (method < pMove)
         tree->randomMove();
     else if (method < pMove + pSwap)
         tree->randomSwap();
     else
         tree->randomRotate();
}

