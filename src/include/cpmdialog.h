#ifndef CPM_DIALOG
#define CPM_DIALOG

#include "cpmrequest.h"
#include <memory>
#include <chrono>
#include <set>
#include <map>
#include <vector>
#include <functional>

class IDialogQueue {
    public:
        virtual ~IDialogQueue();
        virtual string pop() = 0;
        virtual void add(string message) = 0;
        virtual bool isEmpty() = 0;
};

class IRequestCoder {
    public :
        virtual ~IRequestCoder();
        virtual string code(vector<string> message) = 0;
        virtual vector<string> decode(string message) = 0;
};

class IUIDGenerator {
    public:
        virtual ~IUIDGenerator();
        virtual string getNewUID() = 0;
};

class CpmDialog {

    public:

        CpmDialog(shared_ptr<IDialogQueue> senderQueue, shared_ptr<IDialogQueue> receiverQueue, 
            shared_ptr<IRequestCoder> requestCoder, shared_ptr<IUIDGenerator> uidGenerator,
            chrono::milliseconds refreshPeriod = chrono::milliseconds(100), 
            chrono::milliseconds requestTimeout = chrono::milliseconds(5000));
        virtual ~CpmDialog();

        virtual void startDialog();
        virtual void stopDialog();

        void sendRequest(shared_ptr<AbstractCpmCommand> cpmCommand);
        template<typename T, typename... Args>
        shared_ptr<T> sendRequest(Args&&... args);

        template<typename T, typename... Args>
        static uint commandRegister(uint commandID);
        

    protected:

        bool isDialoguing;
        map<string,weak_ptr<AbstractCpmCommand>> pendingRequests;

        virtual void sendResponse(shared_ptr<AbstractCpmCommand> cpmCommand);

        template <typename... Args>
        shared_ptr<AbstractCpmCommand> createCommand(uint commandID, vector<string> argsVect);
        static map<uint, function<shared_ptr<AbstractCpmCommand>(vector<string>)>> registeredCommand;

        shared_ptr<IDialogQueue> mSenderQueue;
        shared_ptr<IDialogQueue> mReceiverQueue;
        shared_ptr<IRequestCoder> mRequestCoder;
        shared_ptr<IUIDGenerator> mUIDGenerator;
        chrono::milliseconds mRefreshPeriod;
        chrono::milliseconds mRequestTimeout;

};

#endif