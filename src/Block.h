#ifndef BLOCK_H_
#define BLOCK_H_

#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Random.h"

/**
 * Defines a block out of the coordinates.
 * This is determined by input and will not modified
 * in the algorithm.
 */
struct Block
{
    double l, w, h; /// corresponding to x, y, z axies.
    int id; /// optional. preserved to link to extra data.

    Block(double _l, double _w, double _h, int _id = -1)
        : l(_l), w(_w), h(_h), id(_id) {}
};

struct RotatableBlock
{
    /// YZX means w'=l, h'=w, l'=h. (l',w',h') is current one.
    enum ROTATE_NAME
    {
        XYZ = 0x012,
        XZY = 0x021,
        YXZ = 0x102,
        YZX = 0x120,
        ZXY = 0x201,
        ZYX = 0x210
    };

    const Block *block;
    ROTATE_NAME rotation;

    RotatableBlock(const Block *_block, ROTATE_NAME _rotation)
        : block(_block), rotation(_rotation) {}

    double getL() const;
    double getW() const;
    double getH() const;
    
    static ROTATE_NAME getRandRotate();
};

inline double RotatableBlock::getL() const
{
    return (rotation & 0xF00) == 0x000 ? block->l : (rotation & 0x0F0) == 0x000 ? block->w : block->h;
}

inline double RotatableBlock::getW() const
{
    return (rotation & 0xF00) == 0x100 ? block->l : (rotation & 0x0F0) == 0x010 ? block->w : block->h;
}

inline double RotatableBlock::getH() const
{
    return (rotation & 0xF00) == 0x200 ? block->l : (rotation & 0x0F0) == 0x020 ? block->w : block->h;
}

inline RotatableBlock::ROTATE_NAME RotatableBlock::getRandRotate()
{
    switch (Random::getInstance().getRandomInt(0, 5))
    {
    case 0: return XYZ;
    case 1: return XZY;
    case 2: return YXZ;
    case 3: return YZX;
    case 4: return ZXY;
    case 5: return ZYX;
    }
    assert(false);
    return XYZ; // return a thing to eliminate warning
}

#endif // BLOCK_H_
