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

const int blockNum = 50;
const int testNum = 100;
const int perturbNum = 10;

const double maxBlockSize = 10;

int main()
{
    for (int i = 0; i < testNum; i++)
    {
        double lastV = INFINITY, curV = INFINITY;
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
        std::cout << "  Volume = " << (lastV = placement.getVolume()) << std::endl;
        for (int j = 0; j < perturbNum; j++)
        {
            std::cout << "  Perturb " << j << std::endl;

            TTree _tree = tree;
            std::cout << "    1. modify and undo" << std::endl;
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
            tree = _tree;
            tree.verify();
            placement = tree.getPlacement();
            placement.verify();
            std::cout << "    Volume = " << (curV = placement.getVolume()) << std::endl;
            assert(lastV == curV);

            std::cout << "    2. modify" << std::endl;
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
            std::cout << "    Volume = " << (lastV = placement.getVolume()) << std::endl;
        }
    }
    return 0;
}

