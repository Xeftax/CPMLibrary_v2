#include "storageunit.h"
#include <filesystem>
#include <fstream>

using namespace cpm;

std::string hexconverter::toHex(const uint32_t uint32) {
    std::stringstream stream;
    stream << std::hex << std::setw(sizeof(uint32_t) * 2) << std::setfill('0') << uint32;
    return stream.str();
}

uint32_t hexconverter::fromHex(const std::string& hexString) {
    std::stringstream stream(hexString);
    uint32_t value;
    stream >> std::hex >> value;
    return value;
}

StorageUnit::StorageUnit(const std::string& path) :
    defaultFolder(path),
    activeFolder(path),
    identifier(Identifier(1, 1, "")),
    nextUID(StoredLiveData<uint32_t>(path / std::filesystem::path(".nextUID"), 1)),
    UIDValidity(StoredLiveData<uint32_t>(path / std::filesystem::path(".UIDValidity"), identifier.uid))
{
    LOG4CXX_DEBUG(logger, "New Cpm StorageUnit created");
}

StorageUnit::~StorageUnit() {
    LOG4CXX_DEBUG(logger, "Cpm StorageUnit destroyed");
}

void StorageUnit::setDialog(std::shared_ptr<Dialog> dialog) {
    communicationGateway = dialog;
    communicationGateway->startDialog();
}

const Identifier StorageUnit::getNextUID() {
    std::lock_guard<std::mutex> lock(mtx);
    uint32_t uid = nextUID.get();
    nextUID.set(uid + 1);
    return Identifier(uid, identifier.uidValidity, "");
}

std::shared_ptr<cpm::ConversationHistory> cpm::StorageUnit::create(const std::string& name) {
    if (communicationGateway == nullptr) {
        throw std::runtime_error("Error creating folder: no communication gateway");
    }
    std::shared_ptr<Select> selectRootFolder = communicationGateway->sendRequest<Select>(defaultFolder);
    if (selectRootFolder->response()->status != Select::Result::OK) {
        throw std::runtime_error("Error selecting default folder: " + defaultFolder);
    }
    std::shared_ptr<Create> createFolder = communicationGateway->sendRequest<Create>(name);
    if (createFolder->response()->status != Create::Result::OK) {
        throw std::runtime_error("Error creating folder");
    }
    std::shared_ptr<Select> selectNewFolder = communicationGateway->sendRequest<Select>(name);
    if (selectNewFolder->response()->status != Select::Result::OK) {
        throw std::runtime_error("Error selecting new folder");
    }
    //return std::make_shared<ConversationHistory>(name, communicationGateway, getNextUID());
}
