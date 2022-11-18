//
// Created by Bartosz Łukasik on 16/11/2022.
//
// Sample implementation that should be used as reference
// for creating ECG algorithms.
//

#include "Sample.h"

// This should be used to define functions that don't belong to
// the class itself, but are required by the class to perform
// certain actions.
namespace
{
    float power(const float a, const int n)
    {
        float result = 1;
        for(int i=0; i<n; i++)
        {
            result*=a;
        }
        return result;
    }
}


Sample::Sample(float a, float b, float c)
{
    Sample::a = a;
    Sample::b = b;
    Sample::c = c;
}

// ImplNote: This is an example of internal method that MUST NOT
// be exposed outside containing class. This is not required outside
// the containing class as its only purpose is to disclose the number of
// roots, not the implementation details.
float Sample::calculateDelta()
{
    return power(b, 2)-(4*a*c);
}

// ImplNote: This is an example of executable method that is exposed
// outside containing class. It handles the entire logic of Sample class
// which is calculating the number of quadratic function roots.
int Sample::getNumberOfRoots()
{
    float delta = Sample::calculateDelta();
    if (delta > 0) {
        return 2;
    } else if (delta == 0) {
        return 1;
    } else {
        return 0;
    }
}