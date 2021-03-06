#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include "SA2.h"
#include "TTree.h"
#include "Block.h"
#include "Placement.h"
#include "TTreedata.h"

/// Report every several steps
void callback(int step, double temperature, double acceptWastedRate, double minWastedRate)
{
    if (step % 1000) return;
    std::clog << " step = " << step
              << " temp = " << temperature
              << " acceptWastedRate = " << acceptWastedRate
              << " minWastedRate = " << minWastedRate
              << std::endl;
}

std::vector<Block> loadBlocks(std::istream &is)
{
    std::vector<Block> ret;
    int n;
    is >> n;
    ret.reserve(n);
    for (int i = 0; i < n; i++)
    {
        double x, y, h;
        is >> x >> y >> h;
        ret.emplace_back(x, y, h, i);
    }
    return ret;
}

SA_2 loadconf(std::istream &is, TTree *tree)
{
    double tSt, tEn;
    int stepCnt;
    double pMove, pSwap;
    
    std::string raw;
    while (is)
    {
        std::string temp;
        getline(is, temp);
        size_t delim = temp.find('#');
        if (delim != std::string::npos) temp.erase(temp.find('#'));
        raw += temp;
    }

    std::istringstream is2(raw);
    is2 >> tSt >> tEn >> stepCnt >> pMove >> pSwap; 
    return SA_2(tSt, tEn, stepCnt, new TTree_data(pMove, pSwap, tree), callback);
}

int main(int argc, char **argv)
{
    std::ifstream inputFile, configFile;
    std::ofstream outputFile;
    if (argc == 4)
    {
        inputFile.open(argv[1]);
        configFile.open(argv[2]);
        outputFile.open(argv[3]);
    }
    if (argc != 4 || ! inputFile.good() || ! configFile.good() || ! outputFile.good())
    {
        std::cout << "Usage : ./3DPlacement_release <inputFile> <configFile> <outputFile>" << std::endl
                  << "<inputFile> can be generated by ./gen" << std::endl;
        return 1;
    }

    auto blocks = loadBlocks(inputFile);

    clock_t clockSt = clock();

    TTree tree;
    tree.initialize(blocks);
    
    SA_2 sa(loadconf(configFile, &tree));

    TTree_data* Tdata = dynamic_cast<TTree_data*>(sa.work());
    assert(Tdata!=NULL);
    Placement placement = Tdata->get_best_Placement();
    double wastedRate = sa.getminans();
    double totVol = placement.getVolume(), netVol = placement.getNetVolume();
    assert(fabs(wastedRate - (1.0 - netVol / totVol)) < 1e-7);

    clock_t clockEn = clock();

    std::cout << "===========================" << std::endl
              << "RESULT: " << std::endl
              << "runtime (exluding time of inputing blocks) = " << double(clockEn - clockSt) / CLOCKS_PER_SEC << "(s)" << std::endl
              << "number of cubes = " << blocks.size() << std::endl
              << "total volume = " << totVol << std::endl
              << "net volume = " << netVol << std::endl
              << "wasted volume = " << (totVol - netVol) << std::endl
              << "wasted rate = " << wastedRate << std::endl
              << "FULL PLACEMENT WILL BE STORED IN " << argv[3] << std::endl;

    outputFile << "# Each line contains the id, the nearest lower-left corner of the block (x, y, z)," << std::endl
               << "# and the length (x), width (y), height(z) of the block. (The block may be rotated)," << std::endl
               << "# i.e. totaly 7 numbers in a line." << std::endl;
    for (const auto &item : placement.getPositions())
        outputFile << item.block.block->id << ' ' << item.x << ' ' << item.y << ' ' << item.z << ' '
                   << item.block.getL() << ' ' << item.block.getW() << ' ' << item.block.getH() << std::endl;
    return 0;
}

