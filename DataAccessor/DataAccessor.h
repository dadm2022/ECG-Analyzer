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
    int m_signalCount = 0; // number of signals in specified record
    std::string m_heaFilePath; //
    std::string m_fileNameWithoutExtension; //

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
    int signalCountGet() const;

    /**
     * @brief Function for getting signals info from loaded record.
     * 
     * @note This function should be called after @ref load().
     *
     * @return std::vector<WFDB_Siginfo> 
     */
    std::vector<WFDB_Siginfo> signalInfoGet() const;

    /**
     * @brief Function for getting signals' data from loaded record.
     * 
     * @note This function should be called after @ref load().
     * 
     * @return std::vector<std::vector<float>> 
     */
    std::vector<std::vector<float>> signalDataGet() const;
};

#endif // ECG_ANALYZER_DATA_ACCESSOR_H
