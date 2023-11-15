#include <cxxtest/TestSuite.h>
#include "message.h"

using namespace cpm;

class CpmMessageTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
    }

    void tearDown() {
        // 
    }

    void testMessageBuilderMinConfig(void) {
        LOG4CXX_DEBUG(logger, "Starting Message Content Builder test (mandatory fields only)");
        cpm::Message::Content messageContent = cpm::Message::Builder()
            .From(utf8Chars)
            .To(utf8Chars)
            .Date(utf8Chars)
            .ConversationID(utf8Chars)
            .ContributionID(utf8Chars)
            .AcceptContact(utf8Chars)
            .IMDNMessageID(utf8Chars)
            .ContentType(utf8Chars)
            .MessageBody(utf8Chars);
        messageContentCheck(messageContent, { "From", "To", "Date", "Conversation-ID", "Contribution-ID", "Accept-Contact", "IMDN-Message-ID", "Content-Type", "Message-body" });
    }

    void testMessageBuilderMaxConfig(void) {
        LOG4CXX_DEBUG(logger, "Starting Message Content Builder test (all fields)");
        cpm::Message::Content messageContent = cpm::Message::Builder()
            .From(utf8Chars)
            .To(utf8Chars)
            .CC(utf8Chars)
            .BCC(utf8Chars)
            .Date(utf8Chars)
            .Subject(utf8Chars)
            .PAssertedService(utf8Chars)
            .ConversationID(utf8Chars)
            .ContributionID(utf8Chars)
            .InReplyToContributionID(utf8Chars)
            .AcceptContact(utf8Chars)
            .IMDNMessageID(utf8Chars)
            .MessageCorrelator(utf8Chars)
            .MessageContext(utf8Chars)
            .ContentType(utf8Chars)
            .MessageBody(utf8Chars);
        messageContentCheck(messageContent, { "From", "To", "CC", "BCC", "Date", "Subject", "P-Asserted-Service", "Conversation-ID", "Contribution-ID", "InReplyTo-Contribution-ID", "Accept-Contact", "IMDN-Message-ID", "Message-Correlator", "Message-Context", "Content-Type", "Message-body" });
    }

    //note: this test is not really useful, since the MessageBuilder does not check for invalid input
    //note: allow creation of message content without passing by the builder


private:

    void messageContentCheck(Message::Content& messageContent, std::vector<std::string> headerKeys) {
        std::map<std::string, std::string> messageContentValues = {
            { "From", messageContent.from },
            { "To", messageContent.to},
            { "CC", messageContent.cc },
            { "BCC", messageContent.bcc },
            { "Date", messageContent.date },
            { "Subject", messageContent.subject },
            { "P-Asserted-Service", messageContent.pAssertedService },
            { "Conversation-ID", messageContent.conversationId },
            { "Contribution-ID", messageContent.contributionId },
            { "InReplyTo-Contribution-ID", messageContent.inReplyToContributionId },
            { "Accept-Contact", messageContent.acceptContact },
            { "IMDN-Message-ID", messageContent.imdnMessageId },
            { "Message-Correlator", messageContent.messageCorrelator },
            { "Message-Context", messageContent.messageContext },
            { "Content-Type", messageContent.contentType },
            { "Message-body", messageContent.messageBody }
        };
        LOG4CXX_DEBUG(logger, "Check message content after construction");
        for (std::string headerKey : headerKeys) {
            TS_ASSERT_EQUALS(messageContentValues[headerKey], utf8Chars);
            messageContentValues.erase(headerKey);
        }
        for (auto& messageContentValue : messageContentValues) {
            TS_ASSERT_EQUALS(messageContentValue.second, "");
        }
        LOG4CXX_DEBUG(logger, "Check message content after formatting");
        std::string formattedMessage = messageContent.format();
        std::stringstream expectedMessage;
        for (std::string headerKey : headerKeys) {
            if (headerKey == "Message-body") continue;
            expectedMessage << headerKey << ": " << utf8Chars << std::endl;
        }
        expectedMessage << std::endl << utf8Chars;
        TS_ASSERT_EQUALS(formattedMessage, expectedMessage.str());
    }

    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_MESSAGE]");

};