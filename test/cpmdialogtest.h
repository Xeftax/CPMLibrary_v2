#include <cxxtest/TestSuite.h>
#include "cpmdialog.h"
#include "cpmrequest.h"
#include "cpmdialogexample.h"

class CpmDialogTestSuite : public CxxTest::TestSuite {

    public:
        void setUp() {
            std::shared_ptr<DialogQueueExample> senderQueue = std::make_shared<DialogQueueExample>();
            std::shared_ptr<DialogQueueExample> receiverQueue = std::make_shared<DialogQueueExample>();
            senderCpmDialog = std::make_shared<CpmDialog>(senderQueue, receiverQueue, std::make_shared<RequestCoderExample>(), std::make_shared<UIDGeneratorExample>());
            recieverCpmDialog = std::make_shared<CpmDialog>(receiverQueue, senderQueue, std::make_shared<RequestCoderExample>(), std::make_shared<UIDGeneratorExample>());
        }

        void tearDown() {
            // 
        }

        void testCpmDialogSend(void) {
            senderCpmDialog->startDialog();
            recieverCpmDialog->startDialog();
            std::shared_ptr<Select> request = std::make_shared<Select>(utf8Chars);
            senderCpmDialog->sendRequest(request);
            TS_ASSERT_EQUALS(request->response()->status, AbstractCpmCommand::Result::OK);
            TS_ASSERT_EQUALS(request->response()->UIDVALIDITY, 765897);
            TS_ASSERT_EQUALS(request->response()->nextUID, 345);
        }

        void testCpmDialogCreateAndSend(void) {
            senderCpmDialog->startDialog();
            recieverCpmDialog->startDialog();
            std::shared_ptr<Select> request = senderCpmDialog->sendRequest<Select>(utf8Chars);
            TS_ASSERT_EQUALS(request->response()->status, AbstractCpmCommand::Result::OK);
            TS_ASSERT_EQUALS(request->response()->UIDVALIDITY, 765897);
            TS_ASSERT_EQUALS(request->response()->nextUID, 345);
        }


    private:

        std::shared_ptr<CpmDialog> senderCpmDialog;
        std::shared_ptr<CpmDialog> recieverCpmDialog;

        std::string utf8Chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // utf-8
};