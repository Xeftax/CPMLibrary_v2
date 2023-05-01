#ifndef CPM_REQUEST
#define CPM_REQUEST

#include <iostream>
#include <string>
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
            Request();
            virtual vector<string> toStringVector();
        };

        struct Response {
            Result status = Result::NONE;
            virtual void fromStringVector(vector<string> response);
        };

        virtual Request* request() = 0;
        virtual Response* response() = 0; 
        virtual void execute() = 0;

    public:

        static uint registrationID;

};

class Select : public AbstractCpmCommand {
    public:
        Select(string& folderName);

        struct Request : AbstractCpmCommand::Request {
            string folderName;
            virtual vector<string> toStringVector();
        };

        struct Response : AbstractCpmCommand::Response {
            uint32_t UIDVALIDITY;
            uint32_t nextUID;
            virtual void fromStringVector(vector<string> response);
        };

        virtual Request* request();
        virtual Response* response();
        virtual void execute();

        static uint registrationID;

    private:
        Request mRequest;
        Response mResponse;
};


#endif