/*
 * Issues: none
 *
 * todo:
 *  badly needs threading
 *
 *
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <functional>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <math.h>

#include <algorithm>


// ----------------------global constant delcrations--------------------------------------------------------------------

const int X = 2;                    // X is the length of the data (how many variables we consider)
const int MAX_THREADS = 4;          // max number of threads the program will use (could switch to system value later)
const int GENOME_LENGTH = 3;        // length of each genome
const int POP_SIZE = 250;           // size of each population
const int SEED_SIZE = POP_SIZE/10;  // how much of each population retained between

//----------------------------------------------------------------------------------------------------------------------


float   RandomFloat(float a, float b);
int     RandomInt(int a, int b);

std::vector <std::function<float(float,float,float)>> functions; // global list of function pointers

class gene
{
    private:

    float sign;
    float k; //scalling factor
    std::function<float(float,float,float)> gene_func;  // pointer to funciton implemented

    int dataIndex;  // index of the data vector this gene operates on
    // ^ this is only held as a record. When the gene is to be passed data this should be used to determine what data


    public:

    gene()
    {
        // set sign
        if(RandomInt(0,100) % 2 == 1)
            sign = (float) -1;
        else
            sign = (float) 1;

        //set k
        k = RandomFloat(-1.0f, 1.0f);

        //set dataIndex
        dataIndex = RandomInt(0,X);

        int accessInt = RandomInt(0,functions.size() - 1);
        //std::cout << "DEBUG: acess int: " << accessInt << " functions size: " << functions.size() << std::endl;
        gene_func = functions[accessInt];

    }


    void set_sign(float in_sign)
    {
        sign = in_sign;
    }
    void set_k(float in_k)
    {
        k = in_k;
    }
    void set_gene_func(std::function<float(float,float,float)> new_func)
    {
        gene_func = new_func;
    }
    /*  this should never be used
    void set_dataIndex(int newIndex)
    {
        dataIndex = newIndex;
    }
    (*/

    int get_dataIndex()
    {
        return dataIndex;
    }

    float produce(float x)  // return the result of the gene
    {
        return gene_func(sign, k, x);
    }

    void mutate(float mut_lvl)  // mutate the gene (mut_level between 0.5-0.000001)
    {
        k = k * (RandomFloat(-1, 1)*mut_lvl);
    }

};

class genome
{

public:

    std::vector <gene*> genes;  // hold all the gene

    float error = 0;    // global average error of the gene from all training data

    genome()
    {
        // fill up the genome with random genes
        for(int i = 0; i < GENOME_LENGTH; i++)
        {
            //genes.push_back(newGene());
            genes.push_back(new gene);
        }
    }
    ~genome()
    {
        // delete all the genes
        for(int i = 0; i < genes.size(); i++)
        {
            delete(genes[i]);
        }
    }

    // run the genome on a data vector
    float produce(std::vector<float> data)
    {
        // go down the genes and sum each result
        float result = 0;

        for(int i = 0; i < genes.size(); i++)
        {
            result += genes[i]->produce(data[genes[i]->get_dataIndex()]);
        }

        return result;
    }

};

float   func_sin_1(float sign, float k, float x);
float   func_exp_1(float sign, float k,  float x);
float func_lin_1(float sign, float k,  float x);

void    set_func_arry();

bool    compareByError(const genome *a, const genome *b);

std::vector<std::vector<float>> loadTrainingData(std::string fileAdr);

genome* gen_genome_from_seed(genome* seed, float mut_lvl);

genome* growGenome(std::vector<std::vector<float>> trainingData);

int main()
{
    std::cout << "DARWIN_TRADER - Running" << std::endl;

    set_func_arry(); // setup the function array
    srand(time(NULL));  // seed RNG


    // load training data
    std::vector<std::vector<float>> trainingData = loadTrainingData("training_data.txt");

    genome* working_gennome = new genome;
    *working_gennome = *growGenome(trainingData);


    std::cout << "DARWIN_TRADER - Ended" << std::endl;
    return 0;
}



// fill the global list of functions THIS MUST HAPPEN BEFORE GENES ARE EVER TOUCHED
void set_func_arry()
{
    functions.push_back(func_exp_1);
    functions.push_back(func_sin_1);
    functions.push_back(func_lin_1);
}


//-------------utility functions----------------------------------------------------------------------------------------

float RandomFloat(float a, float b)
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    float result = a + r;
    return result;
}

// generates random number in the range a-b
int RandomInt(int a, int b)
{
    return (rand() % (b + 1 - a)) + a;
}

//-------------gene functions-------------------------------------------------------------------------------------------

float func_sin_1(float sign, float k,  float x)
{
    float result = sign*sin(k*x);

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

float func_lin_1(float sign, float k,  float x)
{
    float result = sign*(k*x);

    if(isnan(result))
        result = 0;

    return result;
}

//-------------other----------------------------------------------------------------------------------------------------

//load training data from a text file
std::vector<std::vector<float>> loadTrainingData(std::string fileAdr)
{
    std::vector<std::vector<float>> fileData;

    std::string line;
    std::string buf;
    std::string::size_type sz;

    std::ifstream trainingFile(fileAdr);

    if (trainingFile.is_open())
    {
        while (getline (trainingFile,line))   // iterate down the entire file
        {
            std::stringstream ss(line);
            std::vector<float> elements;

            while (ss >> buf)
                elements.push_back(stof(buf,&sz));

            fileData.push_back(elements);
        }
    }

    return fileData;
}

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

// takes a matrix of training data and returns an optimised genome
genome* growGenome(std::vector<std::vector<float>> trainingData)
{
    std::vector<float> errors;

    // create a population
    std::vector<genome*> population;
    for(int i = 0; i < POP_SIZE; i++)
    {
        population.push_back(new genome);
    }

    float mut_lvl = 1;   // start the mutation at 50%

    int breakout = 0;   // used to stop stagnation;
    int allowed_breakouts = 150;
    int breakouts = 0;

    while(mut_lvl > 0)
    {
        // test a population

        float result = 0;
        float real = 0;

        for(int j = 0; j < trainingData.size() - 1; j++)    // we have to exclude the last set because at each stage we need to have a value of "true" which is in the n+1 position
        {
            for(int i = 0; i < population.size(); i++)
            {
                result = population[i]->produce(trainingData[j]);
                real = trainingData[j+1][1];  // look at the next true data value
                population[i]->error += abs(real - result);
            }
        }

        // sort the population lowest error at the front
        sort(population.begin(), population.end(), compareByError);

        // cull the population. remove all but the best SEED_SIZE genomes
        for(int i = SEED_SIZE; i < population.size(); i++)
        {
            delete(population[i]);
        }
        population.erase(population.begin() + SEED_SIZE, population.end());


        if(errors.size() > 0)
            if(population[0]->error == errors.back())
                breakout++;

        if(breakout > 5 && breakouts < allowed_breakouts)
        {
            breakouts++;
            mut_lvl = 1;    // start at a higher mutation again
        }


        errors.push_back(population[0]->error);
        //std::cout<<"Darwin_Trader: Traingin - best error: " << population[0]->error << std::endl;

        // create new genomes from the seeds left in the population
        std::vector<genome*> newTrousers; // new genomes created from seeds
        for(int i = 0; i < population.size(); i++)
        {
            for(int j = 0; j < SEED_SIZE - 1; j++)  // "-1" needed as we're going to leave the seed in place
                newTrousers.push_back(gen_genome_from_seed(population[i],mut_lvl)); // CHANGE MUT_LVL
            population[i]->error = 0;   // reset the error for the next run
        }

        // copy new genes into the population
        for(int i = 0; i < newTrousers.size(); i++)
            population.push_back(newTrousers[i]);

        mut_lvl = mut_lvl - 0.01;
    }

    genome* result_genome = new genome; // copy the result into new memory
    *result_genome = *population[0];

    for(int i = 0; i < population.size(); i++)
    {
        delete(population[i]);
    }

    std::cout << "Best error: " << errors.back() <<std::endl;

    return result_genome;
}




