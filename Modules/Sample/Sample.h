//
// Created by Bartosz Łukasik on 16/11/2022.
//
// Sample header file that should be used as reference
// for creating ECG algorithms.
//

#ifndef ECG_ANALYZER_SAMPLE_H
#define ECG_ANALYZER_SAMPLE_H

#include "iostream"

class Sample {

    public:
    Sample()
    {
        text = "Lorem ipsum";
        number = 2;
        number2 = 3;
    }

    private:
        std::string text;
        int number;
        int number2;

    public:
        std::string getText();
        [[nodiscard]] int sumFields() const;
};

#endif //ECG_ANALYZER_SAMPLE_H
