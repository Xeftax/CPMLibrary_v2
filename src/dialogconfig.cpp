#include "dialogconfig.h"
#include "dialogexample.h"
#include "storageunit.h"
#include <filesystem>

using namespace cpm;

log4cxx::LoggerPtr LocalBackupConfiguration::logger = log4cxx::Logger::getLogger("[obj:static] [CPM_LOCAL_BACKUP_CONFIG]");

std::shared_ptr<StorageUnit> LocalBackupConfiguration::setup(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        LOG4CXX_DEBUG(logger, "Creating folder : " + path);
        std::string parentPath = std::filesystem::path(path).parent_path().string();
        std::string folderName = std::filesystem::path(path).filename().string();
        std::shared_ptr<StorageUnit> parentFolder = std::make_shared<StorageUnit>(parentPath);
        Create(folderName).executeOn(parentFolder);
    }
    std::shared_ptr<DialogQueueExample> senderQueue = std::make_shared<DialogQueueExample>();
    std::shared_ptr<DialogQueueExample> recieverQueue = std::make_shared<DialogQueueExample>();
    std::shared_ptr<StorageUnit> senderStorageFolder = std::make_shared<StorageUnit>(path);
    std::shared_ptr<StorageUnit> recieverStorageFolder = std::make_shared<StorageUnit>(path);
    DialogConfiguration senderConfig {
        recieverStorageFolder,
        senderQueue,
        recieverQueue,
        std::make_shared<RequestCoderExample>(),
        std::make_shared<UIDGeneratorExample>(),
        std::chrono::milliseconds(100),
        std::chrono::milliseconds(5000)
    };
    senderStorageFolder->setDialog(std::make_shared<Dialog>(senderConfig));
    DialogConfiguration recieverConfig {
        senderStorageFolder,
        recieverQueue,
        senderQueue,
        std::make_shared<RequestCoderExample>(),
        std::make_shared<UIDGeneratorExample>(),
        std::chrono::milliseconds(100),
        std::chrono::milliseconds(5000)
    };
    recieverStorageFolder->setDialog(std::make_shared<Dialog>(recieverConfig));
    return senderStorageFolder;
}
