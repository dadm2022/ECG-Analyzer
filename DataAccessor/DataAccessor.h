//
// Created by Laura Kucharska on 20/11/2022.
//
//
//
//

#ifndef ECG_ANALYZER_DATA_ACCESSOR_H
#define ECG_ANALYZER_DATA_ACCESSOR_H

#include <string>
#include <vector>

class DataAccessor {
    std::vector<unsigned char> datBuffer;
    std::vector<std::string> heaBuffer;
    bool loaded = false;
public:
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
     * @brief Get the reference to data buffer.
     * 
     * @return std::vector<unsigned char>& 
     */
    std::vector<unsigned char>& datBufferGet();

    /**
     * @brief Get the reference to header data.
     * Each element of the vector is a new line.
     * 
     * @return std::vector<std::string>& 
     */
    std::vector<std::string>& heaBufferGet();
};

#endif // ECG_ANALYZER_DATA_ACCESSOR_H