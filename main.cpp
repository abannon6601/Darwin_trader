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

int fetchInt();
bool fetchYN();

int main()
{
    std::cout << "DARWIN_TRADER - Running" << std::endl;

    set_func_arry(); // setup the function array

    srand(time(NULL));  // seed RNG

    //______variables_______
    int targetIndex = 0;
    int userInt = 0;

    std::cout << "DARWIN_TRADER - Welcome to the Darwin Trader genetic algorithm demo suite." << std::endl;
    std::cout << "DARWIN_TRADER - Please consult documentation for guide and explanation of results" << std::endl<<std::endl;
    std::cout << "DARWIN_TRADER - Genetic Algorithm launcher" << std::endl;
    std::cout << "DARWIN_TRADER - Load example data (1). Load historical cryptocurrency data (2). Exit (3)." <<std::endl;
    while(userInt < 1 || userInt > 3)
        userInt = fetchInt();

    // switch case variables
    std::vector<std::vector<float>> trainingData;
    genome* working_gennome;
    int good_results;
    float results_ratio;

    switch(userInt)
    {
        case 1:

            trainingData = loadTrainingData("training_data.txt");
            if(trainingData.size() < 1)
            {
                std::cout << "DARWIN_TRADER - Unable to load data. Please ensure files are in correct directories" << std::endl;
                break;
            }

            data_length = trainingData[0].size();   // set the global data size for the genes

            targetIndex = 6;

            working_gennome = growGenome(trainingData);

            good_results = testGenome(trainingData, working_gennome);

            results_ratio = 100*(((float) good_results)/99);

            std::cout<<"DARWIN_TRADER - Best Genome prediction accuracy: " << results_ratio << "%" <<std::endl;

            break;

        case 2:

            // load and parse historical data

            break;

        case 3:
            // handle this
            break;

        default:
            break;
    }


    // load training data



    std::cout << "DARWIN_TRADER - Ended" << std::endl;
    return 0;
}


//-------------other----------------------------------------------------------------------------------------------------

//load training data from a text file TODO: make this robust
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
    {
        fileData.clear();
    }


    return fileData;
}


//-------------UTILITY FUNCTIONS-------(Written by Alan Bannon and so useful they end up in most of my projects)--------

//Fetches an integer from the user and avoids crashes when char string is input into scanf
int fetchInt()
{
    int inputInt;
    bool foundInt = false;
    int result;

    while(!foundInt)
    {
        result = scanf("%d",&inputInt);
        if(result == 0)
        {
            while (fgetc(stdin) != '\n');   // clear the rest of the input buffer
            inputInt = 0;                   // clear junk from the input
            printf("Please enter valid integer\n");
        }
        else
        {
            foundInt = true;
        }
    }
    return inputInt;
}

//Fetches a true/false form the user with y/n
bool fetchYN()
{
    std::string userInput;

    bool flowControl = false;
    while(!flowControl)
    {
        std::getline(std::cin, userInput);

        if(userInput == "Y" || userInput == "y")
        {
            return true;
        }
        if(userInput == "N" || userInput == "n")
        {
            return false;
        }
    }
}





