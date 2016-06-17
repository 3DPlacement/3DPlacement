#include <cmath>
#include <cassert>
#include "SA.h"
#include "TTree.h"
#include "Random.h"

SA::SA(TTree *_tree, double _tSt, double _tEn, int _stepCnt, double _pMove, double _pSwap, SA::callback_t _callback)
    : mTree(_tree), tSt(_tSt), tEn(_tEn), pMove(_pMove), pSwap(_pSwap), stepCnt(_stepCnt), callback(_callback)
{
    assert(mTree != NULL);
    assert(tSt > tEn);
    assert(stepCnt > 0);
    assert(pMove >= 0);
    assert(pSwap >= 0);
    assert(1 - pMove - pSwap >= 0);

    minPlacement = acceptPlacement = mTree->getPlacement();
    minWasted = acceptWasted = 1.0 - acceptPlacement.getNetVolume() / acceptPlacement.getVolume();
    assert(minWasted >= -1e-7 && minWasted <= 1+1e-7);
}

double SA::work()
{
    double t(tSt), fall(pow(tEn / tSt, 1.0 / (stepCnt - 1)));
    for (int i = 0; i < stepCnt; i++, t *= fall)
    {
        double method(Random::getInstance().getRandomDouble(0, 1));
        if (method < pMove)
            mTree->randomMove();
        else if (method < pMove + pSwap)
            mTree->randomSwap();
        else
            mTree->randomRotate();

        Placement curPlacement = mTree->getPlacement();
        double curWasted = 1.0 - curPlacement.getNetVolume() / curPlacement.getVolume();
        assert(curWasted >= -1e-7 && curWasted <= 1+1e-7);
        if (Random::getInstance().getRandomDouble(0, 1) < exp((acceptWasted - curWasted) / t))
        {
            acceptWasted = curWasted;
            acceptPlacement = std::move(curPlacement);
            if (acceptWasted < minWasted)
            {
                minWasted = acceptWasted;
                minPlacement = acceptPlacement;
            }
        } else
            mTree->undo();

        callback(i, t, acceptWasted, minWasted);
    }
    return minWasted;
}

