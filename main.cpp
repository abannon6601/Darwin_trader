/*
 * Issues:
 *  none
 *
 * todo:
 *  badly needs threading
 *
 */

#include <iostream>
#include <sstream>
#include <vector>

#include <functional>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include "gene_functions_H.h"



std::vector<std::vector<float>> loadTrainingData(std::string fileAdr);


int main()
{
    std::cout << "DARWIN_TRADER - Running" << std::endl;

    set_func_arry(); // setup the function array

    srand(time(NULL));  // seed RNG


    // load training data
    std::vector<std::vector<float>> trainingData = loadTrainingData("training_data.txt");

    genome* working_gennome = growGenome(trainingData);

    int good_results = testGenome(trainingData, working_gennome);

    std::cout<<"Best genome error: " << good_results << " correct predictions out of 99" <<std::endl;


    std::cout << "DARWIN_TRADER - Ended" << std::endl;
    return 0;
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
    else
        std::cout<<"FAILED TO OPEN TRAINING DATA"<<std::endl;

    return fileData;
}





