#include "cpmdialog.h"
#include <thread>
#include <chrono>
#include <random>


CpmDialog::CpmDialog(shared_ptr<IDialogQueue> senderQueue, shared_ptr<IDialogQueue> receiverQueue, shared_ptr<IRequestCoder> requestCoder, chrono::milliseconds refreshPeriod, chrono::milliseconds requestTimeout) 
    : mSenderQueue(senderQueue), mReceiverQueue(receiverQueue), mRequestCoder(requestCoder),mRefreshPeriod(refreshPeriod), mRequestTimeout(requestTimeout) {}

CpmDialog::~CpmDialog() { stopDialog(); }

void CpmDialog::startDialog() {
    isDialoguing = true;
    while (isDialoguing) {
        if (!mReceiverQueue->isEmpty()) {
            vector<string> packet = CpmDialog::requestDecoder(mReceiverQueue->pop());
            if (packet.front())
            make_shared
        }
        this_thread::sleep_for(mRefreshPeriod);
    }
}

void CpmDialog::stopDialog() {
    isDialoguing = false;
}

const uint32_t CpmDialog::getNewRequestID() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<uint32_t> dis(0, numeric_limits<uint32_t>::max());

    return dis(gen);
}

template<typename T, typename... Args>
shared_ptr<T> CpmDialog::sendRequest(Args&&... args){
    static_assert(is_base_of<AbstractCpmCommand, T>::value, "Type must derive from ICpmRequest");
    shared_ptr<T> cpmRequest = shared_ptr<T>(new T(std::forward<Args>(args)...));
    sendRequest(cpmRequest);
    return cpmRequest;
}

void CpmDialog::sendRequest(shared_ptr<AbstractCpmCommand> cpmRequest) {
    uint32_t requestID = getNewRequestID();
    mSenderQueue->add(mRequestCoder->code({to_string(requestID)})+cpmRequest->request());
    bool waiting = true;
    while (responseQueue.find(requestID) != responseQueue.end()) {
        this_thread::sleep_for(mRefreshPeriod);
    }
    response = responseQueue[requestID]
    return 
    
}

template<typename T>
bool CpmDialog::commandRegister(uint commandID) {
    registeredCommand.insert(make_pair(commandID,&make_shared<T>));
    return true;
}

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
