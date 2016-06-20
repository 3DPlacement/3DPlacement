#ifndef SA2_H_
#define SA2_H_

#include <cstdlib>
#include "Basedata.h"

/**
 * Simulated Annealing
 */
class SA_2
{
public:
    typedef void(*callback_t)(int step, double temperature, double acceptWastedRate, double minWastedRate);
   
    SA_2(
         double _tSt, /// starting temperature
         double _tEn, /// ending temperature (Temperature falls exponentially)
         int _stepCnt, /// how many steps?
         Base_data*,  /// The data stucture to storage the data
         callback_t _callback = NULL /// callback to report parameters in each step
        );
         
    /// Main work loop
    /// @return : final result
    Base_data* work();
    double getminans() { return minans; }

private:
    double tSt, tEn;
    int stepCnt;
    
    Base_data* curdata;
    
    double oldans,curans,minans;

    callback_t callback;
};
 
#endif // SA2_H_ 
