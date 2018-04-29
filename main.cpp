/*
 * Issues: none
 *
 *
 *
 */

#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <math.h>



// ----------------------global constant delcrations--------------------------------------------------------------------

const int X = 3;              // X is the length of the data (how many variables we consider)
#define MAX_THREADS 4;      // max number of threads the program will use (could switch to system value later)
const int GENOME_LENGTH = 4;  // length of each genome

//----------------------------------------------------------------------------------------------------------------------


float   func_sin_1(float sign, float k, float x);
float   func_exp_1(float sign, float k,  float x);

float   RandomFloat(float a, float b);
int     RandomInt(int a, int b);

void    set_func_arry();

std::vector<std::vector<float>> loadTrainingData(std::string fileAdr);

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


};

class genome
{

public:

    std::vector <gene*> genes;  // hold all the genes

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



    // mutate the genome
    void mutate(float rand_lvl) // rand_lvl dictates how much we mutate by. 1 is complete unrestained, 0  is none
    {
        // TODO
        // go through each gene
        // for each
        //
        // decide if we're replacing completely
        // if not, tweak the scaling function
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


int main()
{
    std::cout << "DARWIN_TRADER - Running" << std::endl;

    set_func_arry(); // setup the function array
    srand(time(NULL));  // seed RNG

    std::vector <std::function<float(float,float,float)>> mydebugfunctions = functions;

    genome* mytestgenome = new genome;

    std::cout << "DARWIN_TRADER - Cleaning up" << std::endl;
    delete(mytestgenome);

    std::cout << "DARWIN_TRADER - End" << std::endl;
    return 0;
}



// fill the global list of functions THIS MUST HAPPEN BEFORE GENES ARE EVER TOUCHED
void set_func_arry()
{
    functions.push_back(func_exp_1);
    functions.push_back(func_sin_1);
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
    return sign*sin(k*x);
}

float func_exp_1(float sign, float k,  float x)
{
    return sign*pow(x,k);
}

std::vector<std::vector<float>> loadTrainingData(std::string fileAdr)
{
    std::string line;
    std::string buf;

    std::ifstream trainingFile(fileAdr);

    if (trainingFile.is_open())
    {

    }
}




