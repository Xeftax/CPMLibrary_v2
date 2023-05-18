#include "cpmdialog.h"
#include "cpmrequest.h"
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/types.h>
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
    listening_thread = thread([this]() {
        while (isDialoguing) {
            if (!mReceiverQueue->isEmpty()) {
            vector<string> message = mRequestCoder->decode(mReceiverQueue->pop());
            string messageUID = message[0];
            auto pendingRequestIterator = pendingRequests.find(messageUID);
            if (pendingRequestIterator != pendingRequests.end()) {
                if (shared_ptr<AbstractCpmCommand> pendingRequest = pendingRequestIterator->second.lock())
                    pendingRequest->response()->fromStringVector(vector<string> {message.begin() + 1, message.end()});
            } else {
                auto createFuncIterator = registeredCommand.find(stoul(message[1]));
                if (createFuncIterator == registeredCommand.end()) 
                    throw range_error("There is no concrete CpmCommand registered in CpmDialog with the id : \""+to_string(stoul(message[1])) +"\"");
                shared_ptr<AbstractCpmCommand> request = createFuncIterator->second(vector<string>(message.begin() + 2, message.end()));
                request->execute();
                vector<string> response = request->response()->toStringVector();
                response.insert(response.begin(),messageUID);
                mSenderQueue->add(mRequestCoder->code(response));
            }
        }
        this_thread::sleep_for(mRefreshPeriod);
        }
    });
}

void CpmDialog::stopDialog() {
    isDialoguing = false;
    if (listening_thread.joinable()) {
        listening_thread.join();
    }
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
