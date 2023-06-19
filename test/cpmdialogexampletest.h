#include <cxxtest/TestSuite.h>
#include "cpmdialogexample.h"

class CpmDialogExampleTestSuite : public CxxTest::TestSuite {

    public:
        void setUp() {
            //
        }

        void tearDown() {
            // 
        }

        void testDialogQueue(void) {
            std::shared_ptr<DialogQueueExample> dialogQueue = std::make_shared<DialogQueueExample>();
            TS_ASSERT_EQUALS(dialogQueue->isEmpty(), true);
            dialogQueue->add(utf8Chars);
            TS_ASSERT_EQUALS(dialogQueue->isEmpty(), false);
            TS_ASSERT_EQUALS(dialogQueue->pop(), utf8Chars);
            TS_ASSERT_EQUALS(dialogQueue->isEmpty(), true);
        }

        void testRequestCoder(void) {
            std::shared_ptr<RequestCoderExample> requestCoder = std::make_shared<RequestCoderExample>();
            size_t utf8Lenght3 = utf8Chars.length() / 3;
            std::vector<std::string> message = {utf8Chars.substr(0, utf8Lenght3), utf8Chars.substr(utf8Lenght3, utf8Lenght3), utf8Chars.substr(2 * utf8Lenght3, utf8Lenght3)};
            TS_ASSERT_EQUALS(requestCoder->decode(requestCoder->code(message)), message);
        }

        void testUIDGenerator(void) {
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
};