#include <cxxtest/TestSuite.h>
#include "groupstate.h"
#include "message.h"

using namespace cpm;

class CpmGroupStateTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
    }

    void tearDown() {
        // 
    }

    void testGroupStateBuilder(void) {
        LOG4CXX_DEBUG(logger, "Starting GroupStateBuilder test");
        GroupStateBuilder groupStateBuilder;
        GroupStateBuilder::Content content = groupStateBuilder.From(utf8Chars)
            .To(utf8Chars)
            .Date(utf8Chars)
            .Conversation_ID(utf8Chars)
            .Contribution_ID(utf8Chars)
            .IMDN_Message_ID(utf8Chars)
            .Content_Type(utf8Chars)
            .Message_Body(utf8Chars);

    }



private:
    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_GROUP_STATE]");
};