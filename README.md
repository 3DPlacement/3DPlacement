
3DPlacement
=========

An algorithm to place 3D blocks in a small volume, referring to *Temporal Floorplanning Using the T-tree Formulation [iccad04]*.


Documentation
---------

***Please refer to `doc/document.pdf` for full documentation.***


Build
---------

First go to the directory

    cd src

Then execute make

    make

Or build in debug mode

    make debug

Or build test

    make test


Run
---------


To do with test data, first go to the `testcase` directory

    cd ../testcase

Execute

    ./gen <n> > blocks.in

to generate <n> blocks to input file blocks.in.

Modify `blocks.conf` to adjust parameters for simulated annealing. See the comments in that file for details.

Execute

    ../src/3DPlacement_release blocks.in blocks.conf blocks.out

to generate a minimal placement into `blocks.out`. Plase do use release build unless doing debugging, because the debug build will not renew random seeds.
