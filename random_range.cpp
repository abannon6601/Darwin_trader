//
// Created by Alan on 01/05/2018.
//

//-------------utility functions----------------------------------------------------------------------------------------
#include <random>

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
