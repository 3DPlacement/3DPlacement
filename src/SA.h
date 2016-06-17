#ifndef SA_H_
#define SA_H_

#include <cstdlib>
#include "Placement.h"

class TTree;

/**
 * Simulated Annealing
 */
class SA
{
public:
    typedef void(*callback_t)(int step, double temperature, double acceptWastedRate, double minWastedRate);

    SA(
       TTree *_tree, /// the tree to modify
       double _tSt, /// starting temperature
       double _tEn, /// ending temperature (Temperature falls exponentially)
       int _stepCnt, /// how many steps?
       double _pMove, /// probability to do TTree::randomMove
       double _pSwap, /// probability to do TTree::randomSwap. (1-pMove-pSwap to do TTree::randomRotate)
       callback_t _callback = NULL /// callback to report parameters in each step
      );

    /// Main work loop
    /// @return : final wasted volume rate
    double work();

    /// Get final placement
    Placement getPlacement() const { return minPlacement; }

private:
    TTree *mTree;
    double tSt, tEn, pMove, pSwap;
    int stepCnt;
    callback_t callback;

    double acceptWasted, minWasted;
    Placement acceptPlacement, minPlacement;
};

#endif // SA_H_

