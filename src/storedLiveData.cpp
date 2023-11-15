#include "storedlivedata.h"
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

using namespace cpm;

template<typename T>
StoredLiveData<T>::StoredLiveData(const std::string& filePath, const T defaultValue) : mFilePath(filePath), mFileDescriptor(-1), mMappedMemory(nullptr) {
    logger = log4cxx::Logger::getLogger("[obj:" + std::to_string(rand()) + "] [CPM_STORED_LIVE_DATA] : " + filePath + " : ");
    LOG4CXX_DEBUG(logger, "New StoredLiveData initialized");
    bool fileExists = std::filesystem::exists(filePath);
    mFileDescriptor = open(mFilePath.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (mFileDescriptor == -1) {
        throw std::runtime_error("Error opening file: " + mFilePath);
    }

    // Truncate the file to the size of T
    ftruncate(mFileDescriptor, sizeof(T));

    // Map the file into memory
    mMappedMemory = mmap(nullptr, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED, mFileDescriptor, 0);
    if (mMappedMemory == MAP_FAILED) {
        throw std::runtime_error("Error mapping file: " + mFilePath);
        close(mFileDescriptor);
    }

    mValue = reinterpret_cast<T*>(mMappedMemory);
    if (!fileExists) {
        set(defaultValue);
        LOG4CXX_DEBUG(logger, "File did not exist, created new file with default value: " + std::to_string(defaultValue));
    }
}

template<typename T>
StoredLiveData<T>::~StoredLiveData() {
    if (mMappedMemory != nullptr) {
        munmap(mMappedMemory, sizeof(T));
        mMappedMemory = nullptr;
    }
    if (mFileDescriptor != -1) {
        close(mFileDescriptor);
    }
}

template<typename T>
void StoredLiveData<T>::set(const T newValue) const {
    if (mMappedMemory == nullptr) {
        throw std::runtime_error("Error: File not mapped.");
    }
    *mValue = newValue;
}

template<typename T>
T StoredLiveData<T>::get() const {
    return (mMappedMemory != nullptr) ? *mValue : 0;
}

template class cpm::StoredLiveData<uint32_t>;
