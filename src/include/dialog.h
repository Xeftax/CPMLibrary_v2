#ifndef CPM_DIALOG
#define CPM_DIALOG

#include "dialogconfig.h"
#include "request.h"
#include <memory>
#include <map>
#include <chrono>
#include <thread>

namespace cpm {

class IDialogQueue {
public:
    virtual ~IDialogQueue();
    virtual std::string pop() = 0;
    virtual void add(std::string message) = 0;
    virtual bool isEmpty() = 0;
};

class IRequestCoder {
public :
    virtual ~IRequestCoder();
    virtual std::string code(std::vector<std::string> message) = 0;
    virtual std::vector<std::string> decode(std::string message) = 0;
};

class IUIDGenerator {
public:
    virtual ~IUIDGenerator();
    virtual std::string getNewUID() = 0;
};

namespace unpack_vector {

template<typename T, size_t... S>
static std::shared_ptr<AbstractCommand> make_shared(std::vector<std::string>& vec, std::index_sequence<S...>) {
    return std::make_shared<T>(vec[S]...);
}

template<typename T, size_t size>
static std::shared_ptr<AbstractCommand> make_shared(std::vector<std::string>& vec) {
    if (vec.size() != size) throw std::invalid_argument("Wrong number of arguments");
    return make_shared<T>(vec, std::make_index_sequence<size>());
}

} // namespace unpack_vector

class Dialog {
public:
    Dialog(DialogConfiguration& configuration);
    virtual ~Dialog();

    virtual void startDialog();
    virtual void stopDialog();

    virtual void sendRequest(std::shared_ptr<AbstractCommand> cpmCommand);

    template<typename CpmCommand, typename... Args>
    std::shared_ptr<CpmCommand> sendRequest(Args&&... args) {
        std::shared_ptr<CpmCommand> cpmRequest = std::make_shared<CpmCommand>(std::forward<Args>(args)...);
        sendRequest(cpmRequest);
        return cpmRequest;
    }

    template<typename T, size_t ArgsNumber>
    static std::string commandRegister(std::string commandID) {
        static_assert(std::is_base_of<AbstractCommand, T>::value, "Class must be derived from AbstractCpmCommand");
        bool result = registeredCommand.insert(std::make_pair(commandID, [] (std::vector<std::string> args) -> std::shared_ptr<AbstractCommand> {
            return unpack_vector::make_shared<T,ArgsNumber>(args);
        })).second;
        if (result == false) throw std::invalid_argument("A CpmCommand is already registered with this id : \""+commandID+"\"");
        return commandID;
    }
    
protected:

    bool isDialoguing;
    std::thread listening_thread;
    std::map<std::string,std::weak_ptr<AbstractCommand>> pendingRequests;

    DialogConfiguration config;

    static std::map<std::string, std::function<std::shared_ptr<AbstractCommand>(std::vector<std::string>)>> registeredCommand;

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:"+std::to_string(rand())+"] [CPM_DIALOG]");
};

} // namespace cpm

#endif // CPM_DIALOG