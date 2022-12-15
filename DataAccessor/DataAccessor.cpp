//
// Created by Laura Kucharska on 28/11/2022.
//

#include "DataAccessor.h"
#include "wfdb.h"

#include <algorithm>
#include <filesystem>
#include <iterator>
#include <iostream>
#include <fstream>
#include <regex>

inline std::string trim(std::string &str) {
    return str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1).erase(0, str.find_first_not_of("\t\n\v\f\r "));
}

void DataAccessor::parseComment() {
    for (auto &line: comment)
    {
        std::smatch match;
        line = trim(line);

        if (std::regex_search(line, match, std::regex(R"(^\d+\s[FM]\s)")))
        {
            std::string mathString = match.str(0);
            auto spaceIndex = mathString.find_first_of(' ');
            age = std::stoi(mathString.substr(0, spaceIndex));
            sex = mathString.at(spaceIndex + 1);
        }
        else if (std::regex_search(line, match, std::regex(R"(^Age:\s+\d+\s+Sex:\s+[FM])")))
        {
            std::string mathString = match.str(0);
            std::regex_search(line, match, std::regex(R"(\d+)"));
            age = std::stoi(match.str(0));
            std::regex_search(line, match, std::regex(R"([FM])"));
            sex = match.str(0).at(0);
        }
    }
}

bool DataAccessor::load(std::string filepath) {
    m_signals.clear();
    m_loaded = false;
    namespace fs = std::filesystem;

    // Windows can deal with '/' as a directory separator.
    std::replace(filepath.begin(), filepath.end(), '\\', '/');

    auto splitPoint = filepath.find_last_of(m_separator);
    auto directoryPath = filepath.substr(0, splitPoint);
    auto fileNameWithExtension = filepath.substr(splitPoint + 1);
    auto m_fileNameWithoutExtension = fileNameWithExtension.substr(0, fileNameWithExtension.find_first_of('.'));

    // Checking .dat file
    std::string datFilePath = directoryPath + m_separator + m_fileNameWithoutExtension + ".dat";
    if (!fs::exists(datFilePath))
        return false;

    // Checking .hea file
    std::string m_heaFilePath = directoryPath + m_separator + m_fileNameWithoutExtension + ".hea";
    if (!fs::exists(m_heaFilePath))
        return false;

    std::ifstream headerFile(m_heaFilePath);

    for (std::string line; getline(headerFile, line);)
        if (line.at(0) == '#') // The line starts with # is a comment
            comment.push_back(line.substr(1));

    char *fname = const_cast<char *>(m_heaFilePath.c_str());
    char *record = const_cast<char *>(m_fileNameWithoutExtension.c_str());

    // Find number of signals in specified record
    for (int i = 0; i < 10; i++)
    {
        m_signalsCount = isigopen(record, fname, nullptr, i);
        if (m_signalsCount > 0)
            break;
    }

    if (m_signalsCount < 1)
        return false;

    m_signals.reserve(m_signalsCount);

    auto signalInfo = new WFDB_Siginfo[m_signalsCount];
    auto sample = new WFDB_Sample[m_signalsCount];

    // Open signal and save data in signalInfo
    if (isigopen(record, fname, signalInfo, m_signalsCount) == m_signalsCount)
    {
        std::vector<float> gain(m_signalsCount);
        std::vector<float> baseline(m_signalsCount);

        for (int i = 0; i < m_signalsCount; i++)
        {
            baseline[i] = static_cast<float>(signalInfo[i].baseline);
            gain[i] = static_cast<float>(signalInfo[i].gain);

            m_signals.push_back({});
            m_signals.at(i).info = signalInfo[i];
            m_signals.at(i).data.reserve(signalInfo[i].nsamp);
        }

        for (int i = 0; i < signalInfo->nsamp; i++)
        {
            if (getvec(sample) < 0)
            {
                std::cout << "error";
                exit(1);
            }

            for (int j = 0; j < m_signalsCount; j++)
                m_signals.at(j).data.push_back(((float) sample[j] - baseline[j]) / gain[j]);
        }

        delete[] signalInfo;
        delete[] sample;
    }

    m_loaded = true;
    parseComment();
    return m_loaded;
}

bool DataAccessor::isLoaded() const {
    return m_loaded;
}

int DataAccessor::signalCountGet() const {
    if (m_loaded)
        return m_signalsCount;

    return 0;
}

Signal& DataAccessor::at(std::size_t index) {

    if (m_loaded && (index < m_signalsCount))
        return m_signals.at(index);

    return dummy;
}

int DataAccessor::ageGet() const {
    if (m_loaded)
        return age;

    return 0;
}

char DataAccessor::sexGet() const {
    if (m_loaded)
        return sex;

    return (char)0;
}

std::vector<std::string> DataAccessor::commentGet() const {
    if (m_loaded)
        return comment;

    return {};
}
