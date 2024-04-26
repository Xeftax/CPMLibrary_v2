#ifndef CPM_DIALOG_CONFIG
#define CPM_DIALOG_CONFIG

#include <log4cxx/logger.h>
#include <memory>
#include <chrono>


namespace cpm {

    class StorageUnit;
    class IDialogQueue;
    class IRequestCoder;
    class IUIDGenerator;

    struct DialogConfiguration {
        const std::shared_ptr<StorageUnit> storageFolder;
        const std::shared_ptr<IDialogQueue> senderQueue;
        const std::shared_ptr<IDialogQueue> receiverQueue;
        const std::shared_ptr<IRequestCoder> requestCoder;
        const std::shared_ptr<IUIDGenerator> uidGenerator;
        const std::chrono::milliseconds refreshPeriod;
        const std::chrono::milliseconds requestTimeout;
    };

    class LocalBackupConfiguration {
    public:
        static std::shared_ptr<StorageUnit> setup(const std::string& path);
    private:
        static log4cxx::LoggerPtr logger;
    };

} // namespace cpm


#endif // CPM_DIALOG_CONFIG