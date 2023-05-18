#ifndef CPM_DIALOG
#define CPM_DIALOG

#include "cpmrequest.h"
#include <cstddef>
#include <memory>
#include <chrono>
#include <set>
#include <map>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>


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

namespace unpack_vector {
    template<typename T, size_t... S>
    static shared_ptr<AbstractCpmCommand> make_shared(vector<string>& vec, index_sequence<S...>) {
        return std::make_shared<T>(vec[S]...);
    }

    template<typename T, size_t size>
    static shared_ptr<AbstractCpmCommand> make_shared(vector<string>& vec) {
        if (vec.size() != size) throw /* choose your error */;
        return make_shared<T>(vec, make_index_sequence<size>());
    }
}

class CpmDialog {

    public:

        CpmDialog(shared_ptr<IDialogQueue> senderQueue, shared_ptr<IDialogQueue> receiverQueue, 
            shared_ptr<IRequestCoder> requestCoder, shared_ptr<IUIDGenerator> uidGenerator,
            chrono::milliseconds refreshPeriod = chrono::milliseconds(100), 
            chrono::milliseconds requestTimeout = chrono::milliseconds(5000));
        virtual ~CpmDialog();

        virtual void startDialog();
        virtual void stopDialog();

        template<typename CpmCommand>
        void sendRequest(shared_ptr<CpmCommand> cpmCommand) {
            static_assert(is_base_of<AbstractCpmCommand, CpmCommand>::value, "Class must be derived from AbstractCpmCommand");
            vector<string> request = cpmCommand->request()->toStringVector();
            vector<string> requestUID = {mUIDGenerator->getNewUID(),to_string(CpmCommand::registrationID)};
            request.insert(request.begin(),requestUID.begin(),requestUID.end());
            mSenderQueue->add(mRequestCoder->code(request));
            pendingRequests.insert(make_pair(requestUID.front(), cpmCommand));
            chrono::milliseconds requestTimeout = mRequestTimeout;
            while (cpmCommand->response()->status == AbstractCpmCommand::Result::NONE && requestTimeout > chrono::milliseconds(0)) {
                this_thread::sleep_for(mRefreshPeriod);
                requestTimeout += -mRefreshPeriod;
            }
            if (requestTimeout <= chrono::milliseconds(0)) throw runtime_error("Request timeout");
        }

        template<typename CpmCommand, typename... Args>
        shared_ptr<CpmCommand> sendRequest(Args&&... args) {
            shared_ptr<CpmCommand> cpmRequest = make_shared<CpmCommand>(forward<Args>(args)...);
            sendRequest<CpmCommand>(cpmRequest);
            return cpmRequest;
        }

        template<typename T, size_t ArgsNumber>
        static uint commandRegister(uint commandID) {
            static_assert(is_base_of<AbstractCpmCommand, T>::value, "Class must be derived from AbstractCpmCommand");
            bool result = registeredCommand.insert(make_pair(commandID, [] (vector<string> args) -> shared_ptr<AbstractCpmCommand> {
                return unpack_vector::make_shared<T,ArgsNumber>(args);
            })).second;
            if (result == false) throw invalid_argument("A CpmCommand is already registered with this id : \""+to_string(commandID) +"\"");
            return commandID;
        }
        

    protected:

        bool isDialoguing;
        thread listening_thread;
        map<string,weak_ptr<AbstractCpmCommand>> pendingRequests;

        static map<uint, function<shared_ptr<AbstractCpmCommand>(vector<string>)>> registeredCommand;

        shared_ptr<IDialogQueue> mSenderQueue;
        shared_ptr<IDialogQueue> mReceiverQueue;
        shared_ptr<IRequestCoder> mRequestCoder;
        shared_ptr<IUIDGenerator> mUIDGenerator;
        chrono::milliseconds mRefreshPeriod;
        chrono::milliseconds mRequestTimeout;
};

#endif