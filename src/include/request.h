#ifndef CPM_REQUEST
#define CPM_REQUEST

#include "logmanager.h"
#include <iostream>
#include <string>
#include <vector>

namespace cpm {

class StorageUnit;

class AbstractCommand {
public:

    enum class Result {
        NONE = 0,
        OK = 200,
        NO = 422,
        BAD = 400
    };

    struct Request {
        virtual std::vector<std::string> toStringVector();
        virtual void fromStringVector(std::vector<std::string> response);
    };

    struct Response {
        Result status = Result::NONE;
        virtual std::vector<std::string> toStringVector();
        virtual void fromStringVector(std::vector<std::string> response);
    };

    virtual Request* request() = 0;
    virtual Response* response() = 0; 
    virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) = 0;

    static std::string registrationID;
    virtual std::string getRegistrationID() = 0;
};

class Create : public AbstractCommand {
public:
    Create(const std::string& folderName);

    struct Request : AbstractCommand::Request {
        std::string folderName = "";
        virtual std::vector<std::string> toStringVector();
        virtual void fromStringVector(std::vector<std::string> response);
    };

    struct Response : AbstractCommand::Response {};

    virtual Request* request() override;
    virtual Response* response() override;
    virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) override;

    static std::string registrationID;
    virtual std::string getRegistrationID() override;

protected:
    Request mRequest;
    Response mResponse;

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:"+std::to_string(rand())+"] [CREATE_REQUEST]");
};

class Select : public AbstractCommand {
public:
    Select(const std::string& folderName);

    struct Request : Create::Request {};

    struct Response : AbstractCommand::Response {
        uint32_t UIDVALIDITY = 0;
        uint32_t nextUID = 0;
        virtual std::vector<std::string> toStringVector();
        virtual void fromStringVector(std::vector<std::string> response);
    };

    virtual Request* request() override;
    virtual Response* response() override;
    virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) override;

    static std::string registrationID;
    virtual std::string getRegistrationID() override;

protected:
    Request mRequest;
    Response mResponse;

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:"+std::to_string(rand())+"] [SELECT_REQUEST]");
};

} // namespace cpm

#endif // CPM_REQUEST