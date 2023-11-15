#ifndef CPM_DIALOG_EXAMPLE
#define CPM_DIALOG_EXAMPLE

#include "dialog.h"
#include <deque>

namespace cpm {

class DialogQueueExample : public IDialogQueue {
    public:
        virtual std::string pop();
        virtual void add(std::string message);
        virtual bool isEmpty();
    private:
        std::deque<std::string> mQueue;
};

class RequestCoderExample : public IRequestCoder {
    public :
        virtual std::string code(std::vector<std::string> message);
        virtual std::vector<std::string> decode(std::string message);
};

class UIDGeneratorExample : public IUIDGenerator {
    public:
        virtual std::string getNewUID();
};

} // namespace cpm

#endif // CPM_DIALOG_EXAMPLE