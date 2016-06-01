#ifndef PLACEMENT_H_
#define PLACEMENT_H_

#include <vector>
#include "Block.h"

/// A compact placement of blocks
class Placement
{
public:
    struct BlockWithPos
    {
        RotatableBlock block;
        double x, y, z;

        BlockWithPos(const RotatableBlock &_block, double _x, double _y, double _z)
            : block(_block), x(_x), y(_y), z(_z) {}
    };

    /// Get all blocks' positions
    const std::vector<BlockWithPos> &getPositions() const { return positions; }

    /// Add one block into this placement
    void addBlock(const RotatableBlock &_block, double _y, double _z);

private:
    /// Check whether two BlockWithPos-s are overlapping with each other in YZ plane
    bool overlappingYZ(double y1, double z1, double w1, double h1, double y2, double z2, double w2, double h2);

    std::vector<BlockWithPos> positions;
};

inline bool Placement::overlappingYZ(double y1, double z1, double w1, double h1, double y2, double z2, double w2, double h2)
{
    return ! (y1 + w1 <= y2 || y1 >= y2 + w2 || z1 + h1 <= z2 || z1 >= z2 + h2);
}

#endif // PLACEMENT_H_
