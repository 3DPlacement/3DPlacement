#include <algorithm>
#include "Placement.h"

void Placement::addBlock(const RotatableBlock &_block, double _y, double _z)
{
    double _x(-INFINITY);
    for (const auto &item : positions)
        if (overlapping2D(item.y, item.z, item.block.getW(), item.block.getH(), _y, _z, _block.getW(), _block.getH()))
            _x = std::max(_x, item.x + item.block.getL());

    positions.emplace_back(_block, _x, _y, _z);
    x1 = std::min(x1, _x), x2 = std::max(x2, _x);
    y1 = std::min(y1, _y), y2 = std::max(y2, _y);
    z1 = std::min(z1, _z), z2 = std::max(z2, _z);
}

#ifndef NDEBUG

void Placement::verify() const
{
    for (const auto &item1 : positions)
    {
        double nearX(0), nearY(0), nearZ(0);
        for (const auto &item2 : positions)
            if (item1.block.block != item2.block.block)
            {
                assert(! overlappingXYZ(item1, item2)); // no overlapping
                if (overlappingYZ(item1, item2) && item2.x < item1.x)
                    nearX = std::max(nearX, item2.x + item2.block.getL());
                if (overlappingXZ(item1, item2) && item2.y < item1.y)
                    nearY = std::max(nearY, item2.y + item2.block.getW());
                if (overlappingXY(item1, item2) && item2.z < item1.z)
                    nearZ = std::max(nearZ, item2.z + item2.block.getH());
            }
        assert(nearX == item1.x && nearY == item1.y && nearZ == item1.z); // is compact
    }
}

#endif // NDEBUG

