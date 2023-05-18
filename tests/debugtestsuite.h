#include <cxxtest/TestSuite.h>
#include <memory>
#include "cpmrequest.h"
#include "cpmdialog.h"
#include "cpmdialogexample.h"

class debugTestSuite : public CxxTest::TestSuite {

    public:

        void setUp() {
            cout << endl << ".START!" << endl;
        }
        void tearDown() {
            cout << ".END!" << endl;
        }

        void testDebug(void) {
            shared_ptr<DialogQueueExample> senderQueue = make_shared<DialogQueueExample>();
            shared_ptr<DialogQueueExample> receiverQueue = make_shared<DialogQueueExample>();
            CpmDialog cpmdialog1(senderQueue,receiverQueue,make_shared<RequestCoderExample>(),make_shared<UIDGeneratorExample>());
            CpmDialog cpmdialog2(receiverQueue,senderQueue,make_shared<RequestCoderExample>(),make_shared<UIDGeneratorExample>());
            cpmdialog1.startDialog();
            cpmdialog2.startDialog();
            string folderName = "babine";
            shared_ptr<Select> request = cpmdialog1.sendRequest<Select>(folderName);
            cout << static_cast<int>(request->response()->status) << endl << request->response()->UIDVALIDITY << endl << request->response()->nextUID << endl;
        }
};