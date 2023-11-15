#ifndef CPM_STORAGE_FOLDER
#define CPM_STORAGE_FOLDER

#include "logmanager.h"
#include "dialog.h"
#include "storedlivedata.h"
#include "object.h"
#include <iomanip>

namespace cpm {

namespace hexconverter {
    static std::string toHex(const uint32_t uint32);
    static uint32_t fromHex(const std::string& hexString);
} // namespace hexconverter

class StorageUnit {
    friend class Create;
    friend class Select;
public:
    StorageUnit(const std::string& path);
    virtual ~StorageUnit();
    void setDialog(std::shared_ptr<Dialog> dialog);

    void create(const std::string& name);
    class ConversationHistory fetch(const uint32_t uid);
    const std::vector<std::string> list();

    const Identifier identifier;
private:
    const StoredLiveData<uint32_t> UIDValidity;
    const StoredLiveData<uint32_t> nextUID;
    const Identifier getNextUID();

    std::string defaultFolder;
    std::string activeFolder;
    std::shared_ptr<Dialog> communicationGateway;
    std::mutex mtx;

    log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("[obj:"+std::to_string(rand())+"] [CPM_STORAGE_UNIT]");
};

} // namespace cpm

#endif // CPM_STORAGE_FOLDER