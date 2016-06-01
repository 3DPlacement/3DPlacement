#include <algorithm>
#include "Placement.h"

void Placement::addBlock(const RotatableBlock &_block, double _y, double _z)
{
    double _x(-INFINITY);
    for (const auto &item : positions)
        if (overlappingYZ(item.y, item.z, item.block.getW(), item.block.getH(), _y, _z, _block.getW(), _block.getH()))
            _x = std::max(_x, item.x + item.block.getL());
    positions.emplace_back(_block, _x, _y, _z);
}

