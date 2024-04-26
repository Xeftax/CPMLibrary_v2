#ifndef CPM_REQUEST
#define CPM_REQUEST

#include "logmanager.h"
#include <iostream>
#include <string>
#include <vector>

namespace cpm {

    class StorageUnit;

    template<typename T>
    using CommandResult = std::pair<T, std::string>;



    class AbstractCommand {
    public:

        enum class Result {
            NONE = 0,
            OK = 200,
            NO = 422,
            BAD = 400
        };

        struct Request {
            Request() = default;
            virtual std::vector<std::string> extract() const;
            virtual void assign(const std::vector<std::string>& request);
        };

        struct Response {
            Response() : status(Result::NONE) {};
            Result status;
            virtual std::vector<std::string> extract() const;
            virtual void assign(const std::vector<std::string>& response);
        };

        virtual const Request* request() const = 0;
        virtual const Response* response() const = 0;
        virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) = 0;

        static std::string registrationID;
        virtual std::string getRegistrationID() = 0;

    protected:
        friend class Dialog;
        std::string* request_uid;
        virtual void setResponse(const std::vector<std::string> response) = 0;
    };

    class Create : public AbstractCommand {
    public:
        Create(const std::string& folderName) : mRequest(folderName), mResponse() {}

        struct Request : AbstractCommand::Request {
            Request(const std::string& folderName) : folderName(folderName) {}
            std::string folderName;
            virtual std::vector<std::string> extract() const;
            virtual void assign(const std::vector<std::string>& request);
        };

        struct Response : AbstractCommand::Response {};

        virtual const Request* request() const override;
        virtual const Response* response() const override;
        virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) override;

        static std::string registrationID;
        virtual std::string getRegistrationID() override;

    protected:
        Request mRequest;
        Response mResponse;
        virtual void setResponse(const std::vector<std::string> response) override;

        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:" + std::to_string(rand()) + "] [CREATE_REQUEST]");
    };

    class Select : public AbstractCommand {
    public:
        Select(const std::string& folderName) : mRequest(folderName), mResponse() {}

        struct Request : Create::Request {
            Request(const std::string& folderName) : Create::Request(folderName) {}
        };

        struct Response : AbstractCommand::Response {
            Response() : AbstractCommand::Response() {}
            Response(const std::vector<std::string>& response) { assign(response); }
            uint32_t UIDVALIDITY;
            uint32_t nextUID;
            uint32_t exists;
            uint32_t recent;
            uint32_t unseen;
            std::vector<std::string> flags;
            std::vector<std::string> permanentFlags;
            std::string access;

            virtual std::vector<std::string> extract() const;
            virtual void assign(const std::vector<std::string>& response);

        };

        virtual const Request* request() const override;
        virtual const Response* response() const override;
        virtual void executeOn(std::shared_ptr<StorageUnit> storageUnit) override;

        static std::string registrationID;
        virtual std::string getRegistrationID() override;

    protected:
        Request mRequest;
        Response mResponse;
        virtual void setResponse(const std::vector<std::string> response) override;

        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:" + std::to_string(rand()) + "] [SELECT_REQUEST]");
    };

} // namespace cpm

#endif // CPM_REQUEST