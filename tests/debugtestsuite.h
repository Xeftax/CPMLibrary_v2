#include <cxxtest/TestSuite.h>
#include <memory>
#include "cpmrequest.h"
#include "cpmdialog.h"
#include "cpmdialogtest.h"


class debugTestSuite : public CxxTest::TestSuite {

    public:

        void setUp() {
            cout << endl << ".START!" << endl;
        }
        void tearDown() {
            cout << ".END!" << endl;
        }

        void testDebug(void) {
            shared_ptr<TestDialogQueue> senderQueue = make_shared<TestDialogQueue>();
            shared_ptr<TestDialogQueue> receiverQueue = make_shared<TestDialogQueue>();
            CpmDialog cpmdialog1(senderQueue,receiverQueue,make_shared<TestRequestCoder>(),make_shared<TestUIDGenerator>());
            CpmDialog cpmdialog2(receiverQueue,senderQueue,make_shared<TestRequestCoder>(),make_shared<TestUIDGenerator>());
            cpmdialog1.startDialog();
            cpmdialog2.startDialog();
            string folderName = "babine";
            shared_ptr<Select> request = cpmdialog1.sendRequest<Select>(folderName);
            cout << static_cast<int>(request->response()->status) << endl << request->response()->UIDVALIDITY << endl << request->response()->nextUID << endl;
        }
};