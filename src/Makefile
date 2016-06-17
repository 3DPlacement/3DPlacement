CC = g++

RELEASE_FLAG = -std=c++11 -DNDEBUG -O2
DEBUG_FLAG = -std=c++11 -g -Wall -fsanitize=address

ifeq ($(OS),Windows_NT)
	RM = del
endif

all : release gen

release : CXXFLAGS = $(RELEASE_FLAG)
release : 3DPlacement_release

debug : CXXFLAGS = $(DEBUG_FLAG)
debug : 3DPlacement_debug

3DPlacement_% : ContourList_%.o Placement_%.o Random_%.o TTree_%.o SA_%.o main_%.o
	$(CC) $(CXXFLAGS) $^ -o $@

test : CXXFLAGS = $(DEBUG_FLAG)
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

SA_%.o : SA.cpp SA.h Placement.h Block.h ContourList.h TTree.h Random.h
	$(CC) $(CXXFLAGS) -c $< -o $@

main_%.o : main.cpp SA.h Placement.h Block.h Random.h ContourList.h TTree.h
	$(CC) $(CXXFLAGS) -c $< -o $@

test.o : test.cpp TTree.h Block.h Random.h Placement.h ContourList.h
	$(CC) $(CXXFLAGS) -c $< -o $@

gen : gen.cpp
	$(CC) $(RELEASE_FLAG) $< -o $@

clean :
	-$(RM) *.o *.gch 3DPlacement_debug 3DPlacement_release test gen

.PHONY : all release debug clean
.SECONDARY :
