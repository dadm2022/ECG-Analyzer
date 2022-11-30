//
// Created by Bartosz Łukasik on 16/11/2022.
//
// Sample implementation that should be used as reference
// for creating ECG algorithms.
//

#include "Sample.h"

// This should be use to define functions that don't belong to
// the class itself, but are required by the class to perform
// certain actions.
namespace
{
    int sum (const int a, const int b)
    {
        return a+b;
    }
}


// Class methods definitions.
std::string Sample::getText()
{
    return text;
}

int Sample::sumFields() const
{
    return sum(number, number2);
}
