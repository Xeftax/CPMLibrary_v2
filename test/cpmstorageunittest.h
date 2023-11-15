#include <cxxtest/TestSuite.h>
#include "dialogconfig.h"
#include "storageunit.h"

using namespace cpm;

class CpmRequestTestSuite : public CxxTest::TestSuite {

public:
        void setUp() {
            LogManager::setupLogger();
        }

        void tearDown() {
            // 
        }

        void testLocalBackupStorage(void) {
            LOG4CXX_DEBUG(logger, "Starting Local Backup Storage Test");
            std::shared_ptr<StorageUnit> storageFolder = LocalBackupConfiguration::setup("/Users/quentin/Documents/CPMLibrary_v2/_storage/userFolder");
            storageFolder->create("test");
        }

    private:
        std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_STORAGE_UNIT]");
};