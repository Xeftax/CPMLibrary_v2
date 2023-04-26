#ifndef CPM_REQUEST
#define CPM_REQUEST

#include <iostream>
#include <vector>

using namespace std;

class AbstractCpmCommand {
    public:
        enum class Result {
            NONE = 0,
            OK = 200,
            NO = 422,
            BAD = 400
        };

        struct Request {
            uint commandID;
        };

        struct Response {
            Result status;
        };

        virtual Request* request() = 0;
        virtual void execute() = 0;
        virtual Response* response() = 0; 
};

class Select : public AbstractCpmCommand {
    public:
        Select(string& folderName);

        struct Request : AbstractCpmCommand::Request {
            string folderName;
        };

        struct Response : AbstractCpmCommand::Response {
            uint32_t UIDVALIDITY;
            uint32_t nextUID; 
        };

        virtual Request* request();
        virtual void execute();
        virtual Response* response();

    private:
        Request mRequest;
        Response mResponse;
        static bool registration;
};


#endif