#include "DataAccessor.h"

#include <filesystem>
#include <fstream>
#include <iterator>

bool DataAccessor::load(const std::string filepath) {
    loaded = false;
    namespace fs = std::filesystem;

    int splitPoint = filepath.find_last_of('/');
    std::string directoryPath = filepath.substr(0, splitPoint);
    std::string fileNameWithExtension = filepath.substr(splitPoint + 1);
    std::string fileNameWithoutExtension = fileNameWithExtension.substr(0, fileNameWithExtension.find_first_of('.'));

    // Reading .dat file
    std::string datFilePath = directoryPath + "/" + fileNameWithoutExtension + ".dat";
    if(!fs::exists(datFilePath)) {
        return false;
    }
    std::ifstream datInput(datFilePath, std::ios::binary);
    std::vector<unsigned char> buffer1(std::istreambuf_iterator<char>(datInput), {});
    datBuffer.clear();
    datBuffer = std::move(buffer1);

    // Reading .hea file
    std::string heaFilePath = directoryPath + "/" + fileNameWithoutExtension + ".hea";
    if(!fs::exists(heaFilePath)) {
        return false;
    }
    std::ifstream nameFileout;
    nameFileout.open(heaFilePath);
    std::string line;
    heaBuffer.clear();
    while(std::getline(nameFileout, line)) {
        heaBuffer.push_back(line);
    }

    loaded = true;
    return loaded;
}

bool DataAccessor::isLoaded() const {
    return loaded;
}

std::vector<unsigned char>& DataAccessor::datBufferGet() {
    return  datBuffer;
}

std::vector<std::string>& DataAccessor::heaBufferGet() {
    return heaBuffer;
}