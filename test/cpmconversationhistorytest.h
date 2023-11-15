#include <cxxtest/TestSuite.h>
#include "conversationhistory.h"
#include "message.h"

using namespace cpm;

class CpmConversationHistoryTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
    }

    void tearDown() {
        // 
    }

    void testConversationHistoryBuilder(void) {
        LOG4CXX_DEBUG(logger, "Starting ConversationHistory Content Builder test");
        cpm::ConversationHistory::Content conversationHistoryContent = cpm::ConversationHistory::Builder()
            .name(utf8Chars);
    }

    void testConversationHistoryAppend(void) {
        cpm::Identifier identifier = cpm::Identifier(1, 1, "babine");
        std::string& utf8Chars = this->utf8Chars;
        cpm::ConversationHistory::Content conversationHistoryContent = cpm::ConversationHistory::Builder()
            .name(utf8Chars);
        cpm::ConversationHistory conversationHistory(identifier, conversationHistoryContent);
        conversationHistory.append<cpm::Message>([utf8Chars](cpm::Message::Builder builder) {
            return builder
                .From(utf8Chars)
                .To(utf8Chars)
                .Date(utf8Chars)
                .ConversationID(utf8Chars)
                .ContributionID(utf8Chars)
                .AcceptContact(utf8Chars)
                .IMDNMessageID(utf8Chars)
                .ContentType(utf8Chars)
                .MessageBody(utf8Chars);
            });
    }

    //note: this test is not really useful, since the MessageBuilder does not check for invalid input
    //note: allow creation of message content without passing by the builder


private:

    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_MESSAGE]");

};