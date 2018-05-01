//
// Created by Alan on 01/05/2018.
//
#pragma once
#ifndef DARWIN_TRADER_MAIN_H
#define DARWIN_TRADER_MAIN_H

#include <vector>
#include <functional>

//-----------------------global constant delcrations--------------------------------------------------------------------

const int X = 6;                    // X is the length of the data (how many variables we consider)
const int MAX_THREADS = 4;          // max number of threads the program will use (could switch to system value later)
const int GENOME_LENGTH = 10;        // length of each genome
const int POP_SIZE = 200;           // size of each population
const int SEED_SIZE = POP_SIZE/20;  // how much of each population retained between culls

//----------------------------------------------------------------------------------------------------------------------

#endif //DARWIN_TRADER_MAIN_H
