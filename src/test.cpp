/**
 * Compile with this file but without main.cpp to
 * perform a test
 */

#include <cassert>
#include <iostream>
#include "Block.h"
#include "TTree.h"
#include "Random.h"
#include "Placement.h"

const int blockNum = 5;
const int testNum = 10;
const int perturbNum = 10;

const double maxBlockSize = 10;

int main()
{
    for (int i = 0; i < testNum; i++)
    {
        std::cout << "Test " << i << std::endl;
        std::vector<Block> blocks;
        blocks.reserve(blockNum);
        for (size_t i = 0; i < blockNum; i++)
            blocks.emplace_back(
                Random::getInstance().getRandomDouble(0, maxBlockSize),
                Random::getInstance().getRandomDouble(0, maxBlockSize),
                Random::getInstance().getRandomDouble(0, maxBlockSize),
                i
            );
        TTree tree;
        std::cout << "  Init tree" << std::endl;
        tree.initialize(blocks);
        tree.verify();
        Placement placement;
        placement = tree.getPlacement();
        placement.verify();
        std::cout << "  Volume = " << placement.getVolume() << std::endl;
        for (int j = 0; j < perturbNum; j++)
        {
            std::cout << "  Perturb " << j << std::endl;
            switch (Random::getInstance().getRandomInt(0, 2))
            {
            case 0:
                tree.randomMove();
                break;
            case 1:
                tree.randomSwap();
                break;
            case 2:
                tree.randomRotate();
                break;
            default:
                assert(false);
            }
            tree.verify();
            placement = tree.getPlacement();
            placement.verify();
            std::cout << "  Volume = " << placement.getVolume() << std::endl;
        }
    }
    return 0;
}

