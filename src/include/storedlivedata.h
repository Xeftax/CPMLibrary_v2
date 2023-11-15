#ifndef CPM_STORED_LIVE_DATA
#define CPM_STORED_LIVE_DATA

#include <string>
#include <log4cxx/logger.h>

namespace cpm {

template<typename T>
class StoredLiveData {
public:
    StoredLiveData(const std::string& filePath, const T defaultValue);
    ~StoredLiveData();

    void set(const T newValue) const;

    T get() const;

private:

    std::string mFilePath;
    int mFileDescriptor;
    void* mMappedMemory;
    T* mValue;

    log4cxx::LoggerPtr logger;
};

} // namespace cpm

#endif // CPM_STORED_LIVE_DATA
