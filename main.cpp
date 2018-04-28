#include <iostream>
#include <vector>
#include <math.h>
#include <functional>

float func_sin_1(float sign, float k, float x);
float func_exp_1(float sign, float k,  float x);

float RandomFloat(float a, float b);

std::vector <std::function<float(float,float,float)>> functions; // global list of function pointers



class gene
{
    private:

    float sign;
    float k; //scalling factor
    std::function<float(float,float,float)> gene_func;  // pointer to funciton implemented

    public:

    int dataIndex;  // index of the data vector this gene operates on


    void set_sign(float in_sign)
    {
        sign = in_sign;
    }
    void set_k(float in_k)
    {
        sign = in_k;
    }
    void set_gene_func(std::function<float(float,float,float)> new_func)
    {
        gene_func = new_func;
    }

    float produce(float x)  // return the result of the gene
    {
        return gene_func(sign, k, x);
    }


};

class genome
{
public:

     std::vector <gene> genes;  // hold all the genes

    gene newGene()  // generated a new random gene
    {
        gene proc_gene;

        proc_gene.set_k(RandomFloat(0f,1f));

    }

};


int main() {





    std::cout << "Hello, World!" << std::endl;
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
    return a + r;
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




