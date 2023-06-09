#include "cpmdialog.h"
#include "cpmrequest.h"

IDialogQueue::~IDialogQueue() {}
IRequestCoder::~IRequestCoder() {}
IUIDGenerator::~IUIDGenerator() {}

CpmDialog::CpmDialog(std::shared_ptr<IDialogQueue> senderQueue, std::shared_ptr<IDialogQueue> receiverQueue, std::shared_ptr<IRequestCoder> requestCoder, std::shared_ptr<IUIDGenerator> uidGenerator, std::chrono::milliseconds refreshPeriod, std::chrono::milliseconds requestTimeout) 
    : mSenderQueue(senderQueue), mReceiverQueue(receiverQueue), mRequestCoder(requestCoder), mUIDGenerator(uidGenerator), mRefreshPeriod(refreshPeriod), mRequestTimeout(requestTimeout) {}

CpmDialog::~CpmDialog() { stopDialog(); }

void CpmDialog::startDialog() {
    isDialoguing = true;
    listening_thread = std::thread([this]() {
        while (isDialoguing) {
            if (!mReceiverQueue->isEmpty()) {
                std::vector<std::string> message = mRequestCoder->decode(mReceiverQueue->pop());
                std::string messageUID = message[0];
                auto pendingRequestIterator = pendingRequests.find(messageUID);
                if (pendingRequestIterator != pendingRequests.end()) {
                    if (std::shared_ptr<AbstractCpmCommand> pendingRequest = pendingRequestIterator->second.lock())
                        pendingRequest->response()->fromStringVector(std::vector<std::string> {message.begin() + 1, message.end()});
                } else {
                    auto createFuncIterator = registeredCommand.find(message[1]);
                    if (createFuncIterator == registeredCommand.end()) 
                        throw std::range_error("There is no concrete CpmCommand registered in CpmDialog with the id : \""+message[1] +"\"");
                    std::shared_ptr<AbstractCpmCommand> request = createFuncIterator->second(std::vector<std::string>(message.begin() + 2, message.end()));
                    request->execute();
                    std::vector<std::string> response = request->response()->toStringVector();
                    response.insert(response.begin(),messageUID);
                    mSenderQueue->add(mRequestCoder->code(response));
                }
            }
        std::this_thread::sleep_for(mRefreshPeriod);
        }
    });
}

void CpmDialog::sendRequest(std::shared_ptr<AbstractCpmCommand> cpmCommand) {
    std::vector<std::string> request = cpmCommand->request()->toStringVector();
    std::vector<std::string> requestUID = { mUIDGenerator->getNewUID(), cpmCommand->getRegistrationID() };
    request.insert(request.begin(),requestUID.begin(),requestUID.end());
    mSenderQueue->add(mRequestCoder->code(request));
    pendingRequests.insert(make_pair(requestUID.front(), cpmCommand));
    std::chrono::milliseconds requestTimeout = mRequestTimeout;
    while (cpmCommand->response()->status == AbstractCpmCommand::Result::NONE && requestTimeout > std::chrono::milliseconds(0)) {
        std::this_thread::sleep_for(mRefreshPeriod);
        requestTimeout += -mRefreshPeriod;
    }
    if (requestTimeout <= std::chrono::milliseconds(0)) throw std::runtime_error("Request timeout");
}

void CpmDialog::stopDialog() {
    isDialoguing = false;
    if (listening_thread.joinable()) {
        listening_thread.join();
    }
}

std::map<std::string, std::function<std::shared_ptr<AbstractCpmCommand>(std::vector<std::string>)>> CpmDialog::registeredCommand;


