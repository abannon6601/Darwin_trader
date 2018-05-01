//
// Created by Alan on 01/05/2018.
// header file for gene_resources
//
#pragma once

#ifndef DARWIN_TRADER_GENE_FUNCTIONS_H_H
#define DARWIN_TRADER_GENE_FUNCTIONS_H_H

#include <vector>


extern std::vector <std::function<float(float,float,float)>> functions;
extern int data_length;

void    set_func_arry();

float   func_sin_1(float sign, float k,  float x);
float   func_sin_2(float sign, float k,  float x);
float   func_exp_1(float sign, float k,  float x);
float   func_exp_2(float sign, float k,  float x);
float   func_lin_1(float sign, float k,  float x);
float   func_const(float sign, float k,  float x);
float   func_log_1(float sign, float k,  float x);

class gene
{
private:

    float sign;
    float k; //scalling factor
    std::function<float(float,float,float)> gene_func;  // pointer to funciton implemented

    int dataIndex;  // index of the data vector this gene operates on
    // ^ this is only held as a record. When the gene is to be passed data this should be used to determine what data


public:

    gene();

    void set_sign(float in_sign);
    void set_k(float in_k);
    void set_gene_func(std::function<float(float,float,float)> new_func);
    int get_dataIndex();
    float produce(float x);
    void mutate(float mut_lvl);

};

class genome
{

public:

    std::vector <gene*> genes;  // hold all the gene
    float error = 0;    // global average error of the gene from all training data

     genome();
    ~genome();

    // run the genome on a data vector
    float produce(std::vector<float> data);

};


bool    compareByError(const genome *a, const genome *b);
genome* gen_genome_from_seed(genome* seed, float mut_lvl);
genome *growGenome(std::vector<std::vector<float>> trainingData, int targetIndex);
int testGenome(std::vector<std::vector<float>> trainingData, genome *testGenome, int targetIndex);



#endif //DARWIN_TRADER_GENE_FUNCTIONS_H_H
