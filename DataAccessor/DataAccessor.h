//
// Created by Laura Kucharska on 28/11/2022.
//
// Header file with DataAccessor class.
//
//

#ifndef ECG_ANALYZER_DATA_ACCESSOR_H
#define ECG_ANALYZER_DATA_ACCESSOR_H

#include "wfdb.h"

#include <string>
#include <vector>
#include <filesystem>

class DataAccessor {
    bool m_loaded = false; // 
    const static char m_separator = '/';
    std::vector<std::string> comment;
    char sex = ' ';
    int age = 0;
    void parseComment();

public:
    /**
     * @brief Signal info and data from record.
     */
    struct Signal {
        WFDB_Siginfo info;
        std::vector<float> data;
    };

private:
    std::vector<Signal> signals;
    Signal dummy; // dummy signal is required to use references.

public:
    /**
     * @brief Construct a new Data Accessor object
     * 
     */
    DataAccessor() = default;

    /**
     * @brief Function for lading record form specified file path.
     * Can be .dat or .hea file.
     * Both .dat and .hea files are required to exist in the same directory.
     * 
     * @param[in] filepath file path.
     * @return true  Succesfully loaded data.
     * @return false Wrong file path.
     */
    bool load(std::string filepath);

    /**
     * @brief Check if data is loaded properly.
     * 
     * @return true  Loaded properly.
     * @return false Not loaded.
     */
    [[nodiscard]] bool isLoaded() const;

    /**
     * @brief Function for getting number of signals from loaded record.
     * 
     * @note This function should be called after @ref load().
     * 
     * @return int Number of signals in record.
     */
    [[nodiscard]] int signalCountGet() const;

    /**
     * @brief Function for getting reference to signal at specified index.
     * Index should be smaller than the value returned from @ref signalCountGet().
     * 
     * @note This function should be called after @ref load().
     * 
     * @param[in] index Specified signal index. 
     * @return Signal& 
     */
    Signal& at(std::size_t index);

    /**
     * @brief Function for getting comment from header file if exists.
     * 
     * @note This function should be called after @ref load().
     * 
     * @return std::vector<std::string>
     */
    [[nodiscard]] std::vector<std::string> commentGet() const;

    /**
     * @brief Function for getting age from comment if exists.
     * 
     * @note This function should be called after @ref load().
     * 
     * @return int 
     */
    [[nodiscard]] int ageGet() const;

    /**
     * @brief Function for getting sex from comment if exists.
     * 
     * This function should be called after @ref load().
     * 
     * @return char 
     */
    [[nodiscard]] char sexGet() const;
};

#endif // ECG_ANALYZER_DATA_ACCESSOR_H
