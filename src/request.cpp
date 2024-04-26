#include "dialog.h"
#include "storageunit.h"
#include <filesystem>
#include <fstream>
#include "request.h"
#include "dialogexample.h"

using namespace cpm;

//AbstractCommand

std::string AbstractCommand::registrationID = "";


std::vector<std::string> AbstractCommand::Request::extract() const { return std::vector<std::string>(); }

void AbstractCommand::Request::assign(const std::vector<std::string>& request) {}


std::vector<std::string> AbstractCommand::Response::extract() const {
    return std::vector<std::string> {std::to_string(static_cast<int>(status))};
}

void AbstractCommand::Response::assign(const std::vector<std::string>& response) {
    if (response.size() < 1) throw std::runtime_error("Invalid response size (expected >= 1)");
    status = static_cast<Result>(std::stoi(response[0]));
}

//Create
std::string Create::registrationID = Dialog::commandRegister<Create, 1>("CREATE");
std::string Create::getRegistrationID() { return registrationID; }

std::vector<std::string> Create::Request::extract() const {
    std::vector<std::string> request = AbstractCommand::Request::extract();
    request.push_back(folderName);
    return request;
}

void Create::Request::assign(const std::vector<std::string>& request) {
    if (request.size() < 1) throw std::runtime_error("Invalid request size (expected >= 1)"); //note: why >= 1 and not == 1 ?
    AbstractCommand::Request::assign(request);
    folderName = request[0];
}

const Create::Request* Create::request() const {
    return &mRequest;
}

const Create::Response* Create::response() const {
    return &mResponse;
}

void Create::executeOn(std::shared_ptr<StorageUnit> storageUnit) {
    LOG4CXX_DEBUG(logger, "Executing Create command");
    mResponse.status = Result::NONE;
    std::filesystem::path newFolder = std::filesystem::path(storageUnit->activeFolder) / std::filesystem::path(mRequest.folderName);
    if (std::filesystem::create_directory(newFolder)) {
        mResponse.status = Result::OK;
    }
    else {
        mResponse.status = Result::NO;
        LOG4CXX_ERROR(logger, "Error while creating " + newFolder.string() + ", folder may already exists");
    }
}

void Create::setResponse(const std::vector<std::string> response) {
    mResponse.assign(response);
}


//Select
std::string Select::registrationID = Dialog::commandRegister<Select, 1>("SELECT");
std::string Select::getRegistrationID() { return registrationID; }

std::vector<std::string> Select::Response::extract() const {
    std::vector<std::string> response = AbstractCommand::Response::extract();
    response.push_back(std::to_string(UIDVALIDITY));
    response.push_back(std::to_string(nextUID));
    response.push_back(std::to_string(exists));
    response.push_back(std::to_string(recent));
    response.push_back(std::to_string(unseen));
    RequestCoderExample vectorCoder;
    response.push_back(vectorCoder.code(flags));
    response.push_back(vectorCoder.code(permanentFlags));
    response.push_back(access);
    return response;
}

void Select::Response::assign(const std::vector<std::string>& response) {
    if (response.size() < 9) throw std::runtime_error("Invalid response size (expected >= 9)");
    AbstractCommand::Response::assign(response);
    UIDVALIDITY = std::stoul(response[1]);
    nextUID = std::stoul(response[2]);
    exists = std::stoul(response[3]);
    recent = std::stoul(response[4]);
    unseen = std::stoul(response[5]);
    RequestCoderExample stringCoder;
    flags = stringCoder.decode(response[6]);
    permanentFlags = stringCoder.decode(response[7]);
    access = response[8];
}

const Select::Request* Select::request() const {
    return &mRequest;
}

const Select::Response* Select::response() const {
    return &mResponse;
}

void Select::executeOn(std::shared_ptr<StorageUnit> storageUnit) {
    LOG4CXX_DEBUG(logger, "Executing Select command");
    mResponse.status = Result::NONE;
    std::map<std::string, std::string> infos;
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
            }
            else {
                LOG4CXX_ERROR(logger, "Info folder not found in " + mRequest.folderName);
                mResponse.status = Result::NO;
                return;
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            LOG4CXX_ERROR(logger, "Error while reading info files in " + mRequest.folderName + " : " + e.what());
            mResponse.status = Result::NO;
            return;
        }
        auto getInfo = [this, &infos, &infoFolder](std::string infoName) -> std::string {
            auto it = infos.find(infoName);
            if (it != infos.end()) {
                return it->second;
            }
            else {
                LOG4CXX_ERROR(logger, "Info file " + infoName + " not found in " + infoFolder.string());
                mResponse.status = Result::NO;
                return "0";
            }
            };
        mResponse.UIDVALIDITY = std::stoul(getInfo("UIDValidity"));
        mResponse.nextUID = std::stoul(getInfo("nextUID"));
    }
    else {
        LOG4CXX_ERROR(logger, mRequest.folderName + " does not exists or is not a directory");
        mResponse.status = Result::NO;
    }
    if (mResponse.status == Result::NONE) {
        mResponse.status = Result::OK;
    }
}

void Select::setResponse(const std::vector<std::string> response) {
    if (response.size() < 3) throw std::runtime_error("Invalid response size (expected >= 3)");
    mResponse.assign(response);
}