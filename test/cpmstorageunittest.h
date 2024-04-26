#include <cxxtest/TestSuite.h>
#include "dialogconfig.h"
#include "storageunit.h"
#include <filesystem>

using namespace cpm;

class StorageUnitTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
        storagePath = std::filesystem::current_path() / std::filesystem::path("_storage");
        std::filesystem::remove_all(storagePath);
        std::filesystem::create_directory(storagePath);
        storagePath.append("user" + std::to_string(std::rand()));
    }

    void tearDown() {
        // 
    }

    void testLocalBackupStorage(void) {
        LOG4CXX_DEBUG(logger, "Starting Local Backup Storage Test");
        std::shared_ptr<StorageUnit> storageFolder = LocalBackupConfiguration::setup(storagePath.string());
        TS_ASSERT_EQUALS(storageFolder->identifier.folderName, storagePath.string());
        TS_ASSERT_EQUALS(storageFolder->identifier.uidValidity, 0);
        TS_ASSERT_EQUALS(storageFolder->identifier.uid, 0);
    }

private:
    std::filesystem::path storagePath;
    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_STORAGE_UNIT]");
};