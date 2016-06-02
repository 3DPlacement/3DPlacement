CC = g++

release : CXXFLAGS = -std=c++11 -DNDEBUG -O2
release : 3DPlacement_release

debug : CXXFLAGS = -std=c++11 -g -Wall
debug : 3DPlacement_debug

3DPlacement_% : ContourList_%.o Placement_%.o Random_%.o TTree_%.o main_%.o
	$(CC) $(CXXFLAGS) $^ -o $@

test : CXXFLAGS = -std=c++11 -g -Wall
test : ContourList_debug.o Placement_debug.o Random_debug.o TTree_debug.o test.o
	$(CC) $(CXXFLAGS) $^ -o $@

ContourList_%.o : ContourList.cpp ContourList.h
	$(CC) $(CXXFLAGS) -c $< -o $@

Placement_%.o : Placement.cpp Placement.h Block.h Random.h
	$(CC) $(CXXFLAGS) -c $< -o $@

Random_%.o : Random.cpp Random.h
	$(CC) $(CXXFLAGS) -c $< -o $@

TTree_%.o : TTree.cpp TTree.h Block.h Random.h Placement.h ContourList.h
	$(CC) $(CXXFLAGS) -c $< -o $@

main_%.o : main.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

test.o : test.cpp TTree.h Block.h Random.h Placement.h ContourList.h
	$(CC) $(CXXFLAGS) -c $< -o $@

