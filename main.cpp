/*
 * Issues:
 *  none
 *
 * todo:
 *
 *
 */

#include <iostream>
#include <sstream>
#include <vector>

#include <functional>
#include <thread>
#include <fstream>
#include <math.h>
#include <algorithm>

#include "gene_functions_H.h"



std::vector<std::vector<float>> loadTrainingData(std::string fileAdr);
int fetchInt();

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
    std::vector<std::vector<float>> growData;
    std::vector<std::vector<float>> evalData;
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

            targetIndex = 5;

            working_gennome = growGenome(trainingData, targetIndex);

            good_results = testGenome(trainingData, working_gennome, targetIndex);

            results_ratio = 100*(((float) good_results)/(trainingData.size()-1));

            std::cout<<"DARWIN_TRADER - Best Genome prediction accuracy: " << results_ratio << "%" <<std::endl;

            break;

        case 2:

            trainingData = loadTrainingData("crypto_combined.txt");
            if(trainingData.size() < 1)
            {
                std::cout << "DARWIN_TRADER - Unable to load data. Please ensure files are in correct directories" << std::endl;
                break;
            }

            data_length = trainingData[0].size();   // set the global data size for the genes

            std::cout << "DARWIN_TRADER - Data from 2017/01/22 to 2018/2/20 available. First half will be used to train a Genome," << std::endl;
            std::cout << "DARWIN_TRADER - the second half will be used to evaluate it. Please choose a target currency:" << std::endl;
            std::cout << "DARWIN_TRADER - Bitcoin(1) Ripple(2) Litecoin(3) Monero(4) Dash(5) Nem(6) Ethereum(7) Waves(8) Ethereum Classic(9)" << std::endl;

            userInt = 0;
            while(userInt < 1 || userInt > 9)
                userInt = fetchInt();

            targetIndex = userInt;


            // split the string into grow and evalutation sections
            for(int i = 0; i < trainingData.size()/2; i++)
            {
                growData.push_back(trainingData[i]);
            }
            for(int i = trainingData.size()/2; i <trainingData.size(); i++)
            {
                evalData.push_back(trainingData[i]);
            }

            working_gennome = growGenome(growData, targetIndex);

            std::cout<<"DARWIN_TRADER - Best Genome name: " << working_gennome <<std::endl;

            good_results = testGenome(evalData, working_gennome, targetIndex);

            results_ratio = 100*(((float) good_results)/(evalData.size()-1));

            std::cout<<"DARWIN_TRADER - Best Genome correct predictions: " << good_results <<std::endl;
            std::cout<<"DARWIN_TRADER - Best Genome prediction accuracy: " << results_ratio << "%" <<std::endl;

            break;

        case 3:
            //exit
            break;

        default:
            break;
    }


    // load training data



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
    {
        fileData.clear();
    }


    return fileData;
}


//-------------UTILITY FUNCTION-------(Written by Alan Bannon and so useful it ends up in most of my projects)----------

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






