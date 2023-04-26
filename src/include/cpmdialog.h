#ifndef CPM_DIALOG
#define CPM_DIALOG

#include "cpmrequest.h"
#include <set>
#include <map>

class IDialogQueue {
    
    public:
        virtual ~IDialogQueue();

        virtual string pop() = 0;
        virtual void add(string) = 0;
        virtual bool isEmpty() = 0;

};

class IRequestCoder {

    public :

        virtual ~IRequestCoder();

        virtual string code(vector<string>) = 0;
        virtual vector<string> decode(string) = 0;
};

class CpmDialog {

    public:

        CpmDialog(shared_ptr<IDialogQueue> senderQueue, shared_ptr<IDialogQueue> receiverQueue, shared_ptr<IRequestCoder> requestCoder,
            chrono::milliseconds refreshPeriod = chrono::milliseconds(100), 
            chrono::milliseconds requestTimeout = chrono::milliseconds(5000));
        virtual ~CpmDialog();

        virtual void startDialog();
        virtual void stopDialog();

        virtual void sendRequest(shared_ptr<AbstractCpmCommand> cpmCommand);
        template<typename T, typename... Args>
        shared_ptr<T> sendRequest(Args&&... args);

    private:

        bool isDialoguing;
        const uint32_t getNewRequestID();
        set<uint32_t> pendingRequestIds;
        map<uint32_t,string> responseQueue;

        shared_ptr<IDialogQueue> mSenderQueue;
        shared_ptr<IDialogQueue> mReceiverQueue;
        shared_ptr<IRequestCoder> mRequestCoder;
        chrono::milliseconds mRefreshPeriod;
        chrono::milliseconds mRequestTimeout;

        template<typename T>
        static bool commandRegister(uint commandID);
        static map<uint,shared_ptr<AbstractCpmCommand> (*)(vector<string>)> registeredCommand;

};

#endif