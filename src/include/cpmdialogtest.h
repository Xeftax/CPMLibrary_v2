#include "cpmdialog.h"
#include <deque>

class TestDialogQueue : public IDialogQueue {
    public:
        virtual string pop();
        virtual void add(string message);
        virtual bool isEmpty();
    private:
        deque<string> mQueue;
};

class TestRequestCoder : public IRequestCoder {
    public :
        virtual string code(vector<string> message);
        virtual vector<string> decode(string message);
};

class TestUIDGenerator : public IUIDGenerator {
    public:
        virtual string getNewUID();
};