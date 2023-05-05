#include "cpmdialog.h"
#include "cpmrequest.h"
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <thread>
#include <chrono>
#include <random>
#include <utility>
#include <tuple>
#include <vector>

IDialogQueue::~IDialogQueue() {}
IRequestCoder::~IRequestCoder() {}
IUIDGenerator::~IUIDGenerator() {}

CpmDialog::CpmDialog(shared_ptr<IDialogQueue> senderQueue, shared_ptr<IDialogQueue> receiverQueue, shared_ptr<IRequestCoder> requestCoder, shared_ptr<IUIDGenerator> uidGenerator, chrono::milliseconds refreshPeriod, chrono::milliseconds requestTimeout) 
    : mSenderQueue(senderQueue), mReceiverQueue(receiverQueue), mRequestCoder(requestCoder), mUIDGenerator(uidGenerator), mRefreshPeriod(refreshPeriod), mRequestTimeout(requestTimeout) {}

CpmDialog::~CpmDialog() { stopDialog(); }

void CpmDialog::startDialog() {
    isDialoguing = true;
    while (isDialoguing) {
        if (!mReceiverQueue->isEmpty()) {
            vector<string> message = mRequestCoder->decode(mReceiverQueue->pop());
            string messageUID = message[0];
            auto pendingRequestIterator = pendingRequests.find(messageUID);
            if (pendingRequestIterator != pendingRequests.end()) {
                if (shared_ptr<AbstractCpmCommand> pendingRequest = pendingRequestIterator->second.lock())
                    pendingRequest->response()->fromStringVector(vector<string> {message.begin() + 1, message.end()});
            } else {
                shared_ptr<AbstractCpmCommand> request = createCommand(stoul(message[1]), vector<string>(message.begin() + 2, message.end()));
                request->execute();
                vector<string> response = request->response()->toStringVector();
                response.insert(response.begin(),messageUID);
                mSenderQueue->add(mRequestCoder->code(response));
            }
        }
        this_thread::sleep_for(mRefreshPeriod);
    }
}

void CpmDialog::stopDialog() {
    isDialoguing = false;
}

template<typename T, typename... Args>
shared_ptr<T> CpmDialog::sendRequest(Args&&... args){
    static_assert(is_base_of<AbstractCpmCommand, T>::value, "Type must derive from ICpmRequest");
    shared_ptr<T> cpmRequest = shared_ptr<T>(new T(forward<Args>(args)...));
    sendRequest(cpmRequest);
    return cpmRequest;
}

void CpmDialog::sendRequest(shared_ptr<AbstractCpmCommand> cpmCommand) {
    vector<string> request = cpmCommand->request()->toStringVector();
    vector<string> requestUID = {mUIDGenerator->getNewUID(),to_string(cpmCommand->registrationID)};
    request.insert(request.begin(),requestUID.begin(),request.end());
    mSenderQueue->add(mRequestCoder->code(request));
    pendingRequests.insert(make_pair(requestUID.front(), cpmCommand));
    while (cpmCommand->response()->status == AbstractCpmCommand::Result::NONE) {
        this_thread::sleep_for(mRefreshPeriod);
    }
}

template <typename... Args>
shared_ptr<AbstractCpmCommand> CpmDialog::createCommand(uint commandID, vector<string> argsVect) {  
    auto createFuncIterator = registeredCommand.find(commandID);
    if (createFuncIterator == registeredCommand.end()) 
        throw range_error("There is no concrete CpmCommand registered in CpmDialog with the id : \""+to_string(commandID) +"\"");
    return createFuncIterator->second(argsVect);
}

map<uint, function<shared_ptr<AbstractCpmCommand>(vector<string>)>> CpmDialog::registeredCommand;


/*string CpmDialog::requestCoder(const list<string> request) {
    string result;
    for (string const& s : request) { result += to_string(s.size()) + ":" + s; }
    return result;
}

list<string> CpmDialog::requestDecoder(const string request) {
    list<string> result;
    int startIndex = 0;
    int lenght = 0;
    while (startIndex <= request.length()) {
        int delimitIndex = request.find(":",startIndex);
        int lenght = stoi(request.substr(startIndex , delimitIndex - startIndex));
        string sub = request.substr(startIndex +1, lenght);
        startIndex += lenght + delimitIndex;
    }
    return result;
}*/
