#include "cpmrequest.h"
#include "cpmdialog.h"

//AbstractCpmCommand

std::string AbstractCpmCommand::registrationID = "";

std::vector<std::string> AbstractCpmCommand::Request::toStringVector() { return std::vector<std::string>();}

void AbstractCpmCommand::Request::fromStringVector(std::vector<std::string> response) { return; }

std::vector<std::string> AbstractCpmCommand::Response::toStringVector() {
    return std::vector<std::string> {std::to_string(static_cast<int>(status))};
}

void AbstractCpmCommand::Response::fromStringVector(std::vector<std::string> response) {
    if (response.size() < 1) throw std::runtime_error("Invalid response size (expected >= 1)");
    status = static_cast<Result>(std::stoi(response[0]));
}

//Select

Select::Select(std::string& folderName) { mRequest.folderName = folderName; }
std::string Select::registrationID = CpmDialog::commandRegister<Select,1>("SELECT");
std::string Select::getRegistrationID() { return registrationID;}

std::vector<std::string> Select::Request::toStringVector() {
    std::vector<std::string> request = AbstractCpmCommand::Request::toStringVector();
    request.push_back(folderName);
    return request;
}

void Select::Request::fromStringVector(std::vector<std::string> response) {
    if (response.size() < 1) throw std::runtime_error("Invalid response size (expected >= 1)");
    AbstractCpmCommand::Request::fromStringVector(response);
    folderName = response[0];
}

std::vector<std::string> Select::Response::toStringVector() {
    std::vector<std::string> response = AbstractCpmCommand::Response::toStringVector();
    response.push_back(std::to_string(UIDVALIDITY));
    response.push_back(std::to_string(nextUID));
    return response;
}

void Select::Response::fromStringVector(std::vector<std::string> response) {
    if (response.size() < 3) throw std::runtime_error("Invalid response size (expected >= 3)");
    AbstractCpmCommand::Response::fromStringVector(response);
    UIDVALIDITY = std::stoul(response[1]);
    nextUID = std::stoul(response[2]);
}

Select::Request* Select::request() {
    return &mRequest;
}

Select::Response* Select::response() {
    return &mResponse;
}

void Select::execute() {
    mResponse.status = Result::OK;
    mResponse.UIDVALIDITY = 765897;
    mResponse.nextUID = 345;
}