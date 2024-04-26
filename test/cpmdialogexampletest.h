#include <cxxtest/TestSuite.h>
#include "dialogexample.h"
#include "message.h"

using namespace cpm;

class CpmDialogExampleTestSuite : public CxxTest::TestSuite {

public:
    void setUp() {
        LogManager::setupLogger();
    }

    void tearDown() {
        // 
    }

    void testDialogQueue(void) {
        LOG4CXX_DEBUG(logger, "Starting DialogQueueExample test");
        std::shared_ptr<DialogQueueExample> dialogQueue = std::make_shared<DialogQueueExample>();
        TSM_ASSERT_EQUALS("DialogQueue is not empty after creation", dialogQueue->isEmpty(), true);
        dialogQueue->add(utf8Chars);
        TSM_ASSERT_EQUALS("DialogQueue is empty after adding a message", dialogQueue->isEmpty(), false);
        TSM_ASSERT_EQUALS("DialogQueue message is not equal to the one added", dialogQueue->pop(), utf8Chars);
        TSM_ASSERT_EQUALS("DialogQueue is not empty after popping the message", dialogQueue->isEmpty(), true);
    }

    void testRequestCoderSimple(void) {
        LOG4CXX_DEBUG(logger, "Starting RequestCoderExample simple test");
        std::shared_ptr<RequestCoderExample> requestCoder = std::make_shared<RequestCoderExample>();
        size_t utf8Lenght3 = utf8Chars.length() / 3;
        std::vector<std::string> message = { utf8Chars.substr(0, utf8Lenght3), utf8Chars.substr(utf8Lenght3, utf8Lenght3), utf8Chars.substr(2 * utf8Lenght3, utf8Lenght3) };
        TSM_ASSERT_EQUALS("The message integrity is not preserved after coding and decoding", requestCoder->decode(requestCoder->code(message)), message);
    }

    void testRequestCoderEmpty(void) {
        LOG4CXX_DEBUG(logger, "Starting RequestCoderExample empty message test");
        std::shared_ptr<RequestCoderExample> requestCoder = std::make_shared<RequestCoderExample>();
        std::vector<std::string> message = {};
        TSM_ASSERT_EQUALS("The message integrity is not preserved after coding and decoding", requestCoder->decode(requestCoder->code(message)), message);
    }

    void testRequestCoderEmptyString(void) {
        LOG4CXX_DEBUG(logger, "Starting RequestCoderExample empty string test");
        std::shared_ptr<RequestCoderExample> requestCoder = std::make_shared<RequestCoderExample>();
        std::vector<std::string> message = { "",utf8Chars,"" };
        TSM_ASSERT_EQUALS("The message integrity is not preserved after coding and decoding", requestCoder->decode(requestCoder->code(message)), message);
    }

    void testRequestCoderCascade(void) {
        LOG4CXX_DEBUG(logger, "Starting RequestCoderExample cascade test");
        std::shared_ptr<RequestCoderExample> requestCoder = std::make_shared<RequestCoderExample>();
        size_t utf8Lenght3 = utf8Chars.length() / 3;
        std::vector<std::string> childMessage = { utf8Chars.substr(0, utf8Lenght3), utf8Chars.substr(utf8Lenght3, utf8Lenght3), utf8Chars.substr(2 * utf8Lenght3, utf8Lenght3) };
        std::vector<std::string> message = { utf8Chars.substr(0, utf8Lenght3), requestCoder->code(childMessage) };
        TSM_ASSERT_EQUALS("The message integrity is not preserved after coding and decoding when it is containing a child vector message", requestCoder->decode(requestCoder->code(message)), message);
        TSM_ASSERT_EQUALS("The child message integrity is not preserved after coding and decoding the parent message", requestCoder->decode(requestCoder->code(message))[1], requestCoder->code(childMessage));

    }

    void testUIDGenerator(void) {
        LOG4CXX_DEBUG(logger, "Starting UIDGeneratorExample test");
        std::shared_ptr<UIDGeneratorExample> uidGenerator = std::make_shared<UIDGeneratorExample>();
        std::set<std::string> uids;
        for (int i = 0; i < 1000; i++) {
            bool result = uids.insert(uidGenerator->getNewUID()).second;
            TSM_ASSERT_EQUALS("At least two UIDs are equal among 1000 generated (index: " + std::to_string(i) + ")", result, true);
            if (!result) { break; }
        }

    }

private:
    std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_DIALOG_EXAMPLE]");
};