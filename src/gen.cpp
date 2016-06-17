/**
 * This program randomly generates the input
 * It generates n blocks, where n is from the command line argument.
 * It outputs to stdout with the format below:
 * The first line contains one number : n
 * Each of the following line contains 3 numbers for each block as : x y h
 */
#include <ctime>
#include <cstdlib>
#include <iostream>

/// gen random double in range [l, r]
inline double randDouble(double l, double r)
{
    return (double)(rand()) / RAND_MAX * (r - l) + l;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int n(0);
    if (argc == 2)
        n = atoi(argv[1]);
    if (n <= 0)
    {
        std::cerr << "Usage : ./gen <n>" << std::endl
                  << ", where <n> is the number of the blocks, and should be >0" << std::endl;
        return 1;
    }

    std::cout << n << std::endl;
    for (int i = 0; i < n; i++)
        std::cout << randDouble(1, 10) << ' ' << randDouble(1, 10) << ' ' << randDouble(1, 10) << std::endl;
    return 0;
}

