#include "dialog.h"

using namespace cpm;

IDialogQueue::~IDialogQueue() {}
IRequestCoder::~IRequestCoder() {}
IUIDGenerator::~IUIDGenerator() {}

Dialog::Dialog(DialogConfiguration& configuration) : config(configuration) {
    LOG4CXX_DEBUG(logger, "New Cpm Dialog created");
}

Dialog::~Dialog() { stopDialog(); }

void Dialog::startDialog() {
    isDialoguing = true;
    listening_thread = std::thread([this]() {
        while (isDialoguing) {
            if (!config.receiverQueue->isEmpty()) {
                std::string codedMessage = config.receiverQueue->pop();
                LOG4CXX_DEBUG(logger, "Received message : " + codedMessage);
                std::vector<std::string> message = config.requestCoder->decode(codedMessage);
                std::string messageUID = message[0];
                auto pendingRequestIterator = pendingRequests.find(messageUID);
                if (pendingRequestIterator != pendingRequests.end()) {
                    LOG4CXX_DEBUG(logger, "Is a response for request : id=" + messageUID);
                    if (std::shared_ptr<AbstractCommand> pendingRequest = pendingRequestIterator->second.lock())
                        pendingRequest->response()->fromStringVector(std::vector<std::string> {message.begin() + 1, message.end()});
                } else {
                    LOG4CXX_DEBUG(logger, "Is a new request : id=" + messageUID);
                    auto createFuncIterator = registeredCommand.find(message[1]);
                    if (createFuncIterator == registeredCommand.end()) 
                        throw std::range_error("There is no concrete CpmCommand registered in Dialog with the id : \""+message[1] +"\"");
                    std::shared_ptr<AbstractCommand> request = createFuncIterator->second(std::vector<std::string>(message.begin() + 2, message.end()));
                    request->executeOn(config.storageFolder);
                    std::vector<std::string> response = request->response()->toStringVector();
                    response.insert(response.begin(),messageUID);
                    config.senderQueue->add(config.requestCoder->code(response));
                }
            }
        std::this_thread::sleep_for(config.refreshPeriod);
        }
    });
    LOG4CXX_DEBUG(logger, "Dialog started");
}

void Dialog::sendRequest(std::shared_ptr<AbstractCommand> cpmCommand) {
    std::vector<std::string> request = cpmCommand->request()->toStringVector();
    std::vector<std::string> requestUID = { config.uidGenerator->getNewUID(), cpmCommand->getRegistrationID() };
    request.insert(request.begin(),requestUID.begin(),requestUID.end());
    std::string codedRequest = config.requestCoder->code(request);
    pendingRequests.insert(make_pair(requestUID.front(), cpmCommand));
    config.senderQueue->add(codedRequest);
    LOG4CXX_DEBUG(logger, "Request sent : " + codedRequest);
    std::chrono::milliseconds requestTimeout = config.requestTimeout;
    while (cpmCommand->response()->status == AbstractCommand::Result::NONE && requestTimeout > std::chrono::milliseconds(0)) {
        std::this_thread::sleep_for(config.refreshPeriod);
        requestTimeout += -config.refreshPeriod;
    }
    if (requestTimeout <= std::chrono::milliseconds(0)) throw std::runtime_error("Request timeout");
    LOG4CXX_DEBUG(logger, "Request response received");
}

void Dialog::stopDialog() {
    isDialoguing = false;
    if (listening_thread.joinable()) {
        listening_thread.join();
    }
    LOG4CXX_DEBUG(logger, "Dialog stopped");
}

std::map<std::string, std::function<std::shared_ptr<AbstractCommand>(std::vector<std::string>)>> Dialog::registeredCommand;


