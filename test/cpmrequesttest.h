#include <cxxtest/TestSuite.h>
#include "request.h"

using namespace cpm;

class CpmRequestTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
        select = std::make_shared<Select>(utf8Chars);
    }

    void tearDown() {
        // 
    }

    void testSelectConstruct(void) {
        LOG4CXX_DEBUG(logger, "Starting Select_Request construction test");
        TS_ASSERT_EQUALS(select->request()->folderName, utf8Chars);
        TS_ASSERT_EQUALS(select->response()->status, AbstractCommand::Result::NONE);
        TS_ASSERT_EQUALS(select->response()->UIDVALIDITY, 0);
        TS_ASSERT_EQUALS(select->response()->nextUID, 0);
        TS_ASSERT_EQUALS(Select::registrationID, "SELECT");
        TS_ASSERT_EQUALS(select->registrationID, "SELECT");
        TS_ASSERT_EQUALS(select->getRegistrationID(), "SELECT");

    }

    void testSelectRequest(void) {
        LOG4CXX_DEBUG(logger, "Starting Select_Request request test");
        // Check request value after construction
        std::vector<std::string> expectedRequest = { utf8Chars };
        TS_ASSERT_EQUALS(select->request()->extract(), expectedRequest);
        // Check request value after modification
        expectedRequest = { utf8Chars.substr(0,utf8Chars.length() / 2) };
        //select->request()->fromStringVector(expectedRequest);
        TS_ASSERT_EQUALS(select->request()->extract(), expectedRequest);
        // Check request bad vector size throws exception
        //TS_ASSERT_THROWS(select->request()->fromStringVector(std::vector<std::string>()), std::runtime_error);
    }

    void testSelectResponse(void) {
        LOG4CXX_DEBUG(logger, "Starting Select_Request response test");
        // Check response value after construction
        std::vector<std::string> expectedResponse = { std::to_string(static_cast<int>(AbstractCommand::Result::NONE)), "0", "0" };
        TS_ASSERT_EQUALS(select->response()->extract(), expectedResponse);
        // Check response value after modification
        expectedResponse = { std::to_string(static_cast<int>(AbstractCommand::Result::OK)), "23432", "987654" };
        //select->response()->fromStringVector(expectedResponse);
        TS_ASSERT_EQUALS(select->response()->extract(), expectedResponse);
        // Check request bad vector size throws exception
        expectedResponse.pop_back();
        //TS_ASSERT_THROWS(select->response()->fromStringVector(expectedResponse), std::runtime_error);
    }

    void testSelectExecute(void) {
        LOG4CXX_DEBUG(logger, "Starting Select_Request execute test");
        //std::shared_ptr<StorageUnit> storageUnit = std::make_shared<StorageUnit>("");
        //select->executeOn();
        //TS_ASSERT_EQUALS(select->response()->status, AbstractCommand::Result::OK);
        //TS_ASSERT_EQUALS(select->response()->UIDVALIDITY, 765897);
        //TS_ASSERT_EQUALS(select->response()->nextUID, 345);
    }

private:
    std::shared_ptr<Select> select;
    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_REQUEST]");
};