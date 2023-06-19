#ifndef CPM_REQUEST
#define CPM_REQUEST

#include <iostream>
#include <string>
#include <vector>

class AbstractCpmCommand {
    
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
        virtual void execute() = 0;

        static uint registrationID;
        uint getRegistrationID();
};

class Select : public AbstractCpmCommand {
    public:
        Select(std::string& folderName);

        struct Request : AbstractCpmCommand::Request {
            std::string folderName;
            virtual std::vector<std::string> toStringVector();
            virtual void fromStringVector(std::vector<std::string> response);

        };

        struct Response : AbstractCpmCommand::Response {
            uint32_t UIDVALIDITY = 0;
            uint32_t nextUID = 0;
            virtual std::vector<std::string> toStringVector();
            virtual void fromStringVector(std::vector<std::string> response);
        };

        virtual Request* request();
        virtual Response* response();
        virtual void execute();

        static uint registrationID;

    protected:
        Request mRequest;
        Response mResponse;
};


#endif
