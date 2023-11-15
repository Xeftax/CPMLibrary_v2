#ifndef CPM_SESSION_HISTORY
#define CPM_SESSION_HISTORY

#include "logmanager.h"
#include <functional>

namespace cpm {

class SessionHistory {
private:
    std::vector<std::shared_ptr<CpmObject>> history;
public:
    template <typename cpmObject>
    shared_ptr<cpmObject> append(std::function<cpmObject::Content(cpmObject::Builder)>){
        
    }
};

} // namespace cpm

#endif // CPM_SESSION_HISTORY