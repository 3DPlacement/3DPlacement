#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdio>
#include "SA2.h"
#include "Random.h"

SA_2::SA_2(double _tSt, double _tEn, int _stepCnt, Base_data* _data, callback_t _callback)
    : tSt(_tSt), tEn(_tEn), stepCnt(_stepCnt), curdata(_data), callback(_callback)
{
    assert(tSt > tEn);
    assert(stepCnt > 0);  
    minans = curans = curdata->getans();
}

Base_data* SA_2::work()
{
    double t(tSt), fall(pow(tEn / tSt, 1.0 / (stepCnt - 1)));
    for (int i = 0; i < stepCnt; i++, t *= fall)
    {
        curdata->move();
        oldans=curans;
        curans=curdata->getans();

        if (curans < minans)
        {
            minans = curans;
            curdata->update();
        }

        if (Random::getInstance().getRandomDouble(0, 1) > exp((oldans - curans) / t))
        {
            curans=oldans;
            curdata->undo();
        }
        if (callback)
            callback(i, t, curans, minans);
    }
    return curdata;
}

