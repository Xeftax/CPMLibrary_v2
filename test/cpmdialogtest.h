#include <cxxtest/TestSuite.h>
#include "dialogexample.h"
#include "dialogconfig.h"
#include "storageunit.h"

using namespace cpm;

class CpmDialogTestSuite : public CxxTest::TestSuite {

    public:
        void setUp() {
            LogManager::setupLogger();
            std::shared_ptr<DialogQueueExample> senderQueue = std::make_shared<DialogQueueExample>();
            std::shared_ptr<DialogQueueExample> receiverQueue = std::make_shared<DialogQueueExample>();
            DialogConfiguration senderCpmDialogConfiguration {
                std::make_shared<StorageUnit>(""),
                senderQueue,
                receiverQueue,
                std::make_shared<RequestCoderExample>(),
                std::make_shared<UIDGeneratorExample>(),
                std::chrono::milliseconds(100),
                std::chrono::milliseconds(5000)
            };
            senderCpmDialog = std::make_shared<Dialog>(senderCpmDialogConfiguration);
            DialogConfiguration recieverCpmDialogConfiguration {
                std::make_shared<StorageUnit>(""),
                receiverQueue,
                senderQueue,
                std::make_shared<RequestCoderExample>(),
                std::make_shared<UIDGeneratorExample>(),
                std::chrono::milliseconds(100),
                std::chrono::milliseconds(5000)
            };
            recieverCpmDialog = std::make_shared<Dialog>(recieverCpmDialogConfiguration);

        }

        void tearDown() {
            // 
        }

        void testCpmDialogSend(void) {
            LOG4CXX_DEBUG(logger, "Starting CpmDialog send test");
            senderCpmDialog->startDialog();
            recieverCpmDialog->startDialog();
            std::shared_ptr<Select> request = std::make_shared<Select>(utf8Chars);
            senderCpmDialog->sendRequest(request);
            TS_ASSERT_EQUALS(request->response()->status, AbstractCommand::Result::OK);
            TS_ASSERT_EQUALS(request->response()->UIDVALIDITY, 765897);
            TS_ASSERT_EQUALS(request->response()->nextUID, 345);
        }

        void testCpmDialogCreateAndSend(void) {
            LOG4CXX_DEBUG(logger, "Starting CpmDialog create and send test");
            senderCpmDialog->startDialog();
            recieverCpmDialog->startDialog();
            std::shared_ptr<Select> request = senderCpmDialog->sendRequest<Select>(utf8Chars);
            TS_ASSERT_EQUALS(request->response()->status, AbstractCommand::Result::OK);
            TS_ASSERT_EQUALS(request->response()->UIDVALIDITY, 765897);
            TS_ASSERT_EQUALS(request->response()->nextUID, 345);
        }


    private:

        std::shared_ptr<Dialog> senderCpmDialog;
        std::shared_ptr<Dialog> recieverCpmDialog;

        std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8

        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[TEST_CPM_DIALOG]");
};