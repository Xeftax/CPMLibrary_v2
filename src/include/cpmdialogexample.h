#ifndef CPM_DIALOG_TEST
#define CPM_DIALOG_TEST

#include "cpmdialog.h"
#include <deque>

class DialogQueueExample : public IDialogQueue {
    public:
        virtual string pop();
        virtual void add(string message);
        virtual bool isEmpty();
    private:
        deque<string> mQueue;
};

class RequestCoderExample : public IRequestCoder {
    public :
        virtual string code(vector<string> message);
        virtual vector<string> decode(string message);
};

class UIDGeneratorExample : public IUIDGenerator {
    public:
        virtual string getNewUID();
};

#endif