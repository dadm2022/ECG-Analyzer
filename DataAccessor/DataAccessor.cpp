//
// Created by Laura Kucharska on 28/11/2022.
//
// Implementation of DataAccessor class.
// 
//

#include "DataAccessor.h"
#include "wfdb.h"

#include <filesystem>
#include <iterator>
#include <iostream>
#include <algorithm>

bool DataAccessor::load(std::string filepath) {
    m_loaded = false;
    m_signalCount = 0;
    namespace fs = std::filesystem;

    // Windows can deal with '/' as a directory separator.
    std::replace(filepath.begin(), filepath.end(), '\\', '/');

    int splitPoint = filepath.find_last_of(m_separator);
    std::string directoryPath = filepath.substr(0, splitPoint);
    std::string fileNameWithExtension = filepath.substr(splitPoint + 1);
    m_fileNameWithoutExtension = fileNameWithExtension.substr(0, fileNameWithExtension.find_first_of('.'));

    // Checking .dat file
    std::string datFilePath = directoryPath + m_separator + m_fileNameWithoutExtension + ".dat";
    if (!fs::exists(datFilePath)) {
        return false;
    }

    // Checking .hea file
    m_heaFilePath = directoryPath + m_separator + m_fileNameWithoutExtension + ".hea";
    if (!fs::exists(m_heaFilePath)) {
        return false;
    }

    char *fname = const_cast<char *>(m_heaFilePath.c_str());
    char *record = const_cast<char *>(m_fileNameWithoutExtension.c_str());

    int signalCount = 0;
    // Find number of signals in specified record
    for (int i = 0; i < 10; i++) {
        signalCount = isigopen(record, fname, nullptr, i);
        if (signalCount > 0) {
            break;
        }
    }

    if (signalCount < 1) {
        return false;
    }

    m_signalCount = signalCount;

    m_loaded = true;
    return m_loaded;
}

bool DataAccessor::isLoaded() const {
    return m_loaded;
}

int DataAccessor::signalCountGet() const {
    if (isLoaded()) {
        return m_signalCount;
    }
    return {};
}

std::vector<WFDB_Siginfo> DataAccessor::signalInfoGet() const {
    if (isLoaded()) {
        char *fname = const_cast<char *>(m_heaFilePath.c_str());
        char *record = const_cast<char *>(m_fileNameWithoutExtension.c_str());
        auto signalInfo = new WFDB_Siginfo[m_signalCount];
        isigopen(record, fname, signalInfo, m_signalCount);
        std::vector<WFDB_Siginfo> signals;
        for (int i = 0; i < m_signalCount; i++) {
            signals.push_back(signalInfo[i]);
        }
        delete[] signalInfo;
        return std::move(signals);
    }
    return {};
}

std::vector<std::vector<float>> DataAccessor::signalDataGet() const {
    if (isLoaded()) {
        char *fname = const_cast<char *>(m_heaFilePath.c_str());
        char *record = const_cast<char *>(m_fileNameWithoutExtension.c_str());

        auto signalInfo = new WFDB_Siginfo[m_signalCount];
        auto sample = new WFDB_Sample[m_signalCount];

        // Open signal and save data in signalInfo
        if (isigopen(record, fname, signalInfo, m_signalCount) == m_signalCount) {
            std::vector<std::vector<float>> dataBuffer(m_signalCount);

            std::vector<float> gain(m_signalCount);
            std::vector<float> baseline(m_signalCount);

            for (int i = 0; i < m_signalCount; i++) {
                baseline[i] = static_cast<float>(signalInfo[i].baseline);
                gain[i] = static_cast<float>(signalInfo[i].gain);
            }

            for (int i = 0; i < signalInfo->nsamp; i++) {
                if (int readSampleError = getvec(sample) < 0) {
                    std::cout << "error";
                    exit(1);
                }

                for (int j = 0; j < m_signalCount; j++) {
                    dataBuffer[j].push_back(((float) sample[j] - baseline[j]) / gain[j]);
                }
            }

            delete[] signalInfo;
            delete[] sample;
            return std::move(dataBuffer);
        }
        delete[] signalInfo;
        delete[] sample;
    }
    return {};
}
