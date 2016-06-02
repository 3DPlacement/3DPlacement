#ifndef PLACEMENT_H_
#define PLACEMENT_H_

#include <cmath>
#include <vector>
#include "Block.h"

/// A compact placement of blocks
class Placement
{
public:
    Placement()
        : x1(INFINITY), x2(-INFINITY), y1(INFINITY), y2(-INFINITY), z1(INFINITY), z2(-INFINITY) {}

    struct BlockWithPos
    {
        RotatableBlock block;
        double x, y, z;

        BlockWithPos(const RotatableBlock &_block, double _x, double _y, double _z)
            : block(_block), x(_x), y(_y), z(_z) {}
    };

    /// Get all blocks' positions
    const std::vector<BlockWithPos> &getPositions() const { return positions; }

    /// Get the volume of the bounding box
    double getVolume() const { return (x2 - x1) * (y2 - y1) * (z2 - z1); }

    /// Add one block into this placement
    void addBlock(const RotatableBlock &_block, double _y, double _z);

#ifndef NDEBUG
    /// Verify some correctness
    void verify() const;
#endif // NDEBUG

private:
    /// Check whether two squres are overlapping with each other in 2D plane (parameters for YZ plane)
    static bool overlapping2D(double y1, double z1, double w1, double h1, double y2, double z2, double w2, double h2);

    /// Check whether two Placement::BlockWithPos are overlapping with each other
    static bool overlappingXYZ(const BlockWithPos &b1, const BlockWithPos &b2);
    static bool overlappingXY(const BlockWithPos &b1, const BlockWithPos &b2);
    static bool overlappingYZ(const BlockWithPos &b1, const BlockWithPos &b2);
    static bool overlappingXZ(const BlockWithPos &b1, const BlockWithPos &b2);

    std::vector<BlockWithPos> positions;

    /// Six boundary
    double x1, x2, y1, y2, z1, z2;
};

inline bool Placement::overlapping2D(double y1, double z1, double w1, double h1, double y2, double z2, double w2, double h2)
{
    return ! (y1 + w1 <= y2 || y1 >= y2 + w2 || z1 + h1 <= z2 || z1 >= z2 + h2);
}

inline bool Placement::overlappingXY(const Placement::BlockWithPos &b1, const Placement::BlockWithPos &b2)
{
    return overlapping2D(b1.x, b1.y, b1.block.getL(), b1.block.getW(), b2.x, b2.y, b2.block.getL(), b2.block.getW());
}

inline bool Placement::overlappingYZ(const Placement::BlockWithPos &b1, const Placement::BlockWithPos &b2)
{
    return overlapping2D(b1.y, b1.z, b1.block.getW(), b1.block.getH(), b2.y, b2.z, b2.block.getW(), b2.block.getH());
}

inline bool Placement::overlappingXZ(const Placement::BlockWithPos &b1, const Placement::BlockWithPos &b2)
{
    return overlapping2D(b1.x, b1.z, b1.block.getL(), b1.block.getH(), b2.x, b2.z, b2.block.getL(), b2.block.getH());
}

inline bool Placement::overlappingXYZ(const Placement::BlockWithPos &b1, const Placement::BlockWithPos &b2)
{
    return overlappingXY(b1, b2) && overlappingYZ(b1, b2) && overlappingXZ(b1, b2);
}

#endif // PLACEMENT_H_
