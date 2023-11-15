#include "dialog.h"
#include "storageunit.h"
#include <filesystem>
#include <fstream>

using namespace cpm;

//AbstractCommand

std::string AbstractCommand::registrationID = "";

std::vector<std::string> AbstractCommand::Request::toStringVector() { return std::vector<std::string>();}

void AbstractCommand::Request::fromStringVector(std::vector<std::string> response) { return; }

std::vector<std::string> AbstractCommand::Response::toStringVector() {
    return std::vector<std::string> {std::to_string(static_cast<int>(status))};
}

void AbstractCommand::Response::fromStringVector(std::vector<std::string> response) {
    if (response.size() < 1) throw std::runtime_error("Invalid response size (expected >= 1)");
    status = static_cast<Result>(std::stoi(response[0]));
}

//Create

Create::Create(const std::string& folderName) { mRequest.folderName = folderName; }
std::string Create::registrationID = Dialog::commandRegister<Create,1>("CREATE");
std::string Create::getRegistrationID() { return registrationID;}

std::vector<std::string> Create::Request::toStringVector() {
    std::vector<std::string> request = AbstractCommand::Request::toStringVector();
    request.push_back(folderName);
    return request;
}

void Create::Request::fromStringVector(std::vector<std::string> request) {
    if (request.size() < 1) throw std::runtime_error("Invalid request size (expected >= 1)");
    AbstractCommand::Request::fromStringVector(request);
    folderName = request[0];
}

Create::Request* Create::request() {
    return &mRequest;
}

Create::Response* Create::response() {
    return &mResponse;
}

void Create::executeOn(std::shared_ptr<StorageUnit> storageUnit) {
    LOG4CXX_DEBUG(logger, "Executing Create command");
    mResponse.status = Result::NONE;
    std::filesystem::path newFolder = std::filesystem::path(storageUnit->activeFolder) / std::filesystem::path(mRequest.folderName);
    if (std::filesystem::create_directory(newFolder)) {
        mResponse.status = Result::OK;
    } else {
        mResponse.status = Result::NO;
        LOG4CXX_ERROR(logger, "Error while creating "+newFolder.string()+", folder may already exists");
    }
}


//Select

Select::Select(const std::string& folderName) { mRequest.folderName = folderName; }
std::string Select::registrationID = Dialog::commandRegister<Select,1>("SELECT");
std::string Select::getRegistrationID() { return registrationID;}

std::vector<std::string> Select::Response::toStringVector() {
    std::vector<std::string> response = AbstractCommand::Response::toStringVector();
    response.push_back(std::to_string(UIDVALIDITY));
    response.push_back(std::to_string(nextUID));
    return response;
}

void Select::Response::fromStringVector(std::vector<std::string> response) {
    if (response.size() < 2) throw std::runtime_error("Invalid response size (expected >= 3)");
    AbstractCommand::Response::fromStringVector(response);
    UIDVALIDITY = std::stoul(response[1]);
    nextUID = std::stoul(response[2]);
}

Select::Request* Select::request() {
    return &mRequest;
}

Select::Response* Select::response() {
    return &mResponse;
}

void Select::executeOn(std::shared_ptr<StorageUnit> storageUnit) {
    LOG4CXX_DEBUG(logger, "Executing Select command");
    mResponse.status = Result::NONE;
    std::map<std::string,std::string> infos;
    if (std::filesystem::exists(mRequest.folderName) && std::filesystem::is_directory(mRequest.folderName)) {
        std::filesystem::path infoFolder = std::filesystem::path(mRequest.folderName) / std::filesystem::path(".info");
        try {
            if (std::filesystem::exists(infoFolder) && std::filesystem::is_directory(infoFolder)) {
                for (const auto& entry : std::filesystem::directory_iterator(infoFolder)) {
                    if (std::filesystem::is_regular_file(entry)) {
                        std::string fileName = entry.path().filename();
                        size_t lastUnderscorePos = fileName.rfind("_");
                        if (lastUnderscorePos != std::string::npos) {
                            std::string key = fileName.substr(0, lastUnderscorePos);
                            std::string value = fileName.substr(lastUnderscorePos + 1);
                            infos.insert(std::make_pair(key, value));
                        }
                    }
                }
            } else {
                LOG4CXX_ERROR(logger, "Info folder not found in "+mRequest.folderName);
                mResponse.status = Result::NO;
                return;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            LOG4CXX_ERROR(logger, "Error while reading info files in "+mRequest.folderName+" : "+e.what());
            mResponse.status = Result::NO;
            return;
        }
        auto getInfo = [this,&infos,&infoFolder](std::string infoName) -> std::string {
            auto it = infos.find(infoName);
            if (it != infos.end()) {
                return it->second;
            } else {
                LOG4CXX_ERROR(logger, "Info file "+infoName+" not found in "+infoFolder.string());
                mResponse.status = Result::NO;
                return "0";
            }
        };
        mResponse.UIDVALIDITY = std::stoul(getInfo("UIDValidity"));
        mResponse.nextUID = std::stoul(getInfo("nextUID"));
    } else {
        LOG4CXX_ERROR(logger, mRequest.folderName+" does not exists or is not a directory");
        mResponse.status = Result::NO;
    }
    if (mResponse.status == Result::NONE) {
        mResponse.status = Result::OK;
    }
}

