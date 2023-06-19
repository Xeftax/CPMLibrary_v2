#include <cxxtest/TestSuite.h>
#include "cpmrequest.h"

class CpmRequestTestSuite : public CxxTest::TestSuite {

    public:
        void setUp() {
            select = std::make_shared<Select>(utf8Chars);
        }

        void tearDown() {
            // 
        }

        void testSelectConstruct(void) {
            TS_ASSERT_EQUALS(select->request()->folderName, utf8Chars);
            TS_ASSERT_EQUALS(select->response()->status, AbstractCpmCommand::Result::NONE);
            TS_ASSERT_EQUALS(select->response()->UIDVALIDITY, NULL);
            TS_ASSERT_EQUALS(select->response()->nextUID, NULL);
            TS_ASSERT_EQUALS(Select::registrationID, 1);
            TS_ASSERT_EQUALS(select->registrationID, 1);
            TS_ASSERT_EQUALS(select->getRegistrationID(), 1);
            
        }

        void testSelectRequest(void) {
            // Check request value after construction
            std::vector<std::string> expectedRequest = { utf8Chars };
            TS_ASSERT_EQUALS(select->request()->toStringVector(), expectedRequest);
            // Check request value after modification
            expectedRequest = {utf8Chars.substr(0,utf8Chars.length()/2)};
            select->request()->fromStringVector(expectedRequest);
            TS_ASSERT_EQUALS(select->request()->toStringVector(), expectedRequest);
            // Check request modification throws exception
            TS_ASSERT_THROWS(select->request()->fromStringVector(std::vector<std::string>()), std::runtime_error);
        }

        void testSelectResponse(void) {
            // Check response value after construction
            std::vector<std::string> expectedResponse = { std::to_string(static_cast<int>(AbstractCpmCommand::Result::NONE)), "0", "0" };
            TS_ASSERT_EQUALS(select->response()->toStringVector(), expectedResponse);
            // Check response value after modification
            expectedResponse = { std::to_string(static_cast<int>(AbstractCpmCommand::Result::OK)), "23432", "987654" };
            select->response()->fromStringVector(expectedResponse);
            TS_ASSERT_EQUALS(select->response()->toStringVector(), expectedResponse);
            // Check response modification throws exception
            expectedResponse.pop_back();
            TS_ASSERT_THROWS(select->response()->fromStringVector(expectedResponse), std::runtime_error);
        }

        void testSelectExecute(void) {
            select->execute();
            TS_ASSERT_EQUALS(select->response()->status, AbstractCpmCommand::Result::OK);
            TS_ASSERT_EQUALS(select->response()->UIDVALIDITY, 765897);
            TS_ASSERT_EQUALS(select->response()->nextUID, 345);
        }

    private:
        std::shared_ptr<Select> select;
        std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8
};