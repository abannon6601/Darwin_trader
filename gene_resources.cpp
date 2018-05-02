//
// Created by Alan on 13/04/2018.
//


#include <math.h>
#include <functional>
#include <algorithm>
#include <iostream>
#include <thread>
#include <future>

#include "random_range.h"
#include "gene_functions_H.h"
#include "main.h"

std::vector <std::function<float(float,float,float)>> functions; // global list of function pointers

int data_length;                    // data_length is the length of the data (how many variables we consider)

gene::gene() {
    // set sign
    if(RandomInt(0,100) % 2 == 1)
        sign = (float) -1;
    else
        sign = (float) 1;

    //set k
    k = RandomFloat(-1.0f, 1.0f);

    //set dataIndex
    dataIndex = RandomInt(0,data_length);

    int accessInt = RandomInt(0,functions.size() - 1);
    //std::cout << "DEBUG: acess int: " << accessInt << " functions size: " << functions.size() << std::endl;
    gene_func = functions[accessInt];
}
void gene::set_sign(float in_sign) {
    sign = in_sign;
}
void gene::set_k(float in_k) {
    k = in_k;
}
void gene::set_gene_func(std::function<float(float,float,float)> new_func) {
    gene_func = new_func;
}
int gene::get_dataIndex() {
    return dataIndex;
}
float gene::produce(float x) {
    return gene_func(sign, k, x);
}
void gene::mutate(float mut_lvl) {
    k = k + (RandomFloat(-1, 1)*mut_lvl);
}

genome::genome() {
    // fill up the genome with random genes
    for(int i = 0; i < GENOME_LENGTH; i++)
    {
        //genes.push_back(newGene());
        genes.push_back(new gene);
    }
}
genome::~genome(){
    // delete all the genes
    for(int i = 0; i < genes.size(); i++)
    {
        delete(genes[i]);
    }
}
float genome::produce(std::vector<float> data) {
    // go down the genes and sum each result
    float result = 0;

    for(int i = 0; i < genes.size(); i++)
    {
        result += genes[i]->produce(data[genes[i]->get_dataIndex()]);
    }

    return result;
}


void test_population(std::vector<std::vector<float>> trainingData,std::vector<genome*> population, int targetIndex, int numberInSequence, int threadSequenceLength);


// fill the global list of functions THIS MUST HAPPEN BEFORE GENES ARE EVER TOUCHED
void set_func_arry()
{
    functions.push_back(func_exp_1);
    functions.push_back(func_exp_2);
    functions.push_back(func_sin_1);
    functions.push_back(func_sin_2);
    functions.push_back(func_lin_1);
    functions.push_back(func_const);
    functions.push_back(func_log_1);

}

//---------------gene functions-----------------------------------------------------------------------------------------

float func_sin_1(float sign, float k,  float x)
{
    float result = sign*sin(k*x);

    if(isnan(result))
        result = 0;

    return result;
}

float func_sin_2(float sign, float k,  float x)
{
    float result = sign*k*sin(x);

    if(isnan(result))
        result = 0;

    return result;
}

float func_exp_1(float sign, float k,  float x)
{
    float result = sign*pow(x,k);

    if(isnan(result))
        result = 0;

    return result;
}

float func_exp_2(float sign, float k,  float x)
{
    float result = sign*pow(k,x);

    if(isnan(result))
        result = 0;

    return result;
}

float func_lin_1(float sign, float k,  float x)
{
    float result = sign*(k*x);

    if(isnan(result))
        result = 0;

    return result;
}

float func_const(float sign, float k,  float x)
{
    float result = sign*k;

    if(isnan(result))
        result = 0;

    return result;
}

float func_log_1(float sign, float k,  float x)
{
    float result = sign*k*log(x);

    if(isnan(result))
        result = 0;

    return result;
}

//----------------------------------------------------------------------------------------------------------------------

// generate a mutated genome from a given seed genome and mutation level
genome* gen_genome_from_seed (genome* seed, float mut_lvl)
{
    // TODO
    // go through each gene
    // for each
    //
    // decide if we're replacing completely
    // if not, tweak the scaling function
    float decidor;
    std::vector<gene*> seed_genes = seed->genes;
    std::vector<gene*> gen_genes;

    for(int i =0; i < seed_genes.size(); i++)
    {
        decidor = RandomFloat(0, 1);
        if(decidor > mut_lvl)
        {   // replace the gene
            gen_genes.push_back(new gene);
        }
        else
        {   // mutate the gene
            // create new gene, copy the old gene, mutate it.
            gene* temp = new gene;
            *temp = *seed_genes[i];
            temp->mutate(mut_lvl);
            gen_genes.push_back(temp);
        }
    }

    // create a new genome and copy the new genes into it
    genome* result_genome = new genome;
    result_genome->genes = gen_genes;

    return result_genome;

}

//sorting function for comparing genomes
bool compareByError(const genome *a, const genome *b)
{
    return a->error < b->error;
}

// returns the number of correct predictions of a genome
inline bool SameSign(float a, float b) {
    return a*b >= 0.0f;
}
int testGenome(std::vector<std::vector<float>> trainingData, genome *testGenome, int targetIndex)
{
    int result = 0;

    std::vector<float> dataTrend;
    for(int i = 0; i < trainingData.size()-1; i++)
    {
        dataTrend.push_back(trainingData[i+1][targetIndex] - trainingData[i][targetIndex]);
    }
    std::vector<float> resultTrend;
    for(int i = 0; i < trainingData.size()-1; i++)
    {
        resultTrend.push_back(testGenome->produce(trainingData[i+1]) - testGenome->produce(trainingData[i]));
    }

    for(int i = 0; i < resultTrend.size(); i++)
    {
        if(SameSign(dataTrend[i],resultTrend[i])) // if the predictions agree
            result++;
    }

    return result;
}

// simple function called to join a thread
void do_join(std::thread& t)
{
    t.join();
}

// takes a matrix of training data and returns an optimised genome
genome *growGenome(std::vector<std::vector<float>> trainingData, int targetIndex)
{
    std::vector<float> errors;

    // create a population
    std::vector<genome*> population;
    for(int i = 0; i < POP_SIZE; i++)
    {
        population.push_back(new genome);
    }

    int MAX_THREADS = std::thread::hardware_concurrency();
    if(MAX_THREADS < 1)
        MAX_THREADS = 4;    // the call may not always return, so 4 is a safe bet.

    std::cout<<"DARWIN_TRADER - Growning algorithm with " << MAX_THREADS << " threads supported"<<std::endl;
    std::vector<std::thread> training_threads;

    float mut_lvl = 1;   // start the mutation at 50% (average mutation is 1/2 of mut_lvl, as mut_lvl acts on a flat dist)

    int breakout = 0;   // used to stop stagnation;
    int allowed_breakouts = 50;
    int breakouts = 0;

    while(mut_lvl > 0)
    {
        // test a population

        // fire threads
        for(int i =0; i < MAX_THREADS; i++)
        {
            training_threads.push_back(std::thread(test_population, trainingData, population, targetIndex, i, MAX_THREADS));
        }
        // join threads
        std::for_each(training_threads.begin(),training_threads.end(),do_join);   // ensure all threads are complete before continuing
        training_threads.clear();


        // sort the population lowest error at the front
        sort(population.begin(), population.end(), compareByError);

        // cull the population. remove all but the best SEED_SIZE genomes
        for(int i = SEED_SIZE; i < population.size(); i++)
        {
            delete(population[i]);
        }
        population.erase(population.begin() + SEED_SIZE, population.end());

        // if we have exactly the same error as before
        if(errors.size() > 0)
            if(population[0]->error == errors.back())
                breakout++; // increase the counter

        // if the top error hasn't improved over five mutate/culls
        if(breakout > 5 && breakouts < allowed_breakouts)
        {
            breakout = 0;
            breakouts++;
            mut_lvl = 1;    // start at a higher mutation again
        }

        errors.push_back(population[0]->error);
        //std::cout<<"Darwin_Trader: Training - best error: " << population[0]->error << std::endl;

        // create new genomes from the seeds left in the population
        std::vector<genome*> newTrousers; // new genomes created from seeds
        for(int i = 0; i < population.size(); i++)
        {
            for(int j = 0; j < SEED_SIZE - 1; j++)  // "-1" needed as we're going to leave the seed in place
                newTrousers.push_back(gen_genome_from_seed(population[i],mut_lvl));
            population[i]->error = 0;   // reset the error for the next run
        }

        // copy new genes into the population
        for(int i = 0; i < newTrousers.size(); i++)
            population.push_back(newTrousers[i]);

        mut_lvl = mut_lvl - 0.01;
    }

    genome* result_genome = population[0];

    for(int i = 1; i < population.size(); i++)
    {
        delete(population[i]);
    }

    //std::cout << "Best error: " << errors.back() <<std::endl;


    std::cout << "DARWIN_TRADER - " << errors.size()*POP_SIZE <<" Genomes considered over " << errors.size() << " mutate//cull cycles" <<std::endl;


    return result_genome;
}

// runs in a thread to test a population against a dataset. Note that numberInSequence must be 0 indexed
void test_population(std::vector<std::vector<float>> trainingData,std::vector<genome*> population, int targetIndex, int numberInSequence, int threadSequenceLength)
{
    int processLength = population.size()/threadSequenceLength;
    int startPoint = numberInSequence*(processLength); // find where wew should start processing the population

    float result = 0;
    float real = 0;

    for(int j = 0; j < trainingData.size() - 1; j++)    // we have to exclude the last set because at each stage we need to have a value of "true" which is in the n+1 position
    {
        for(int i = startPoint; i < startPoint + processLength; i++)
        {
            result = population[i]->produce(trainingData[j]);
            real = trainingData[j+1][targetIndex];  // look at the next true data value
            population[i]->error += abs(real - result);
        }
    }

}