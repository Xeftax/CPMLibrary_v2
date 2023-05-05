#include "cpmrequest.h"
#include "cpmdialog.h"
#include <array>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

uint AbstractCpmCommand::registrationID = 0;
uint AbstractCpmCommand::getRegistrationID() { return registrationID;}

vector<string> AbstractCpmCommand::Request::toStringVector() { return vector<string>();}

void AbstractCpmCommand::Request::fromStringVector(vector<string> response) { return; }

vector<string> AbstractCpmCommand::Response::toStringVector() {
    return vector<string> {to_string(static_cast<int>(status))};
}

void AbstractCpmCommand::Response::fromStringVector(vector<string> response) {
    status = static_cast<Result>(stoi(response[0]));
}

Select::Select(string& folderName) { mRequest.folderName = folderName; }
uint Select::registrationID = CpmDialog::commandRegister<Select,1>(1) ;

vector<string> Select::Request::toStringVector() {
    vector<string> request = AbstractCpmCommand::Request::toStringVector();
    request.push_back(folderName);
    return request;
}

void Select::Request::fromStringVector(vector<string> response) {
    AbstractCpmCommand::Request::fromStringVector(response);
    folderName = response[0];
}

vector<string> Select::Response::toStringVector() {
    vector<string> response = AbstractCpmCommand::Response::toStringVector();
    response.push_back(to_string(UIDVALIDITY));
    response.push_back(to_string(nextUID));
    return response;
}

void Select::Response::fromStringVector(vector<string> response) {
    AbstractCpmCommand::Response::fromStringVector(response);
    UIDVALIDITY = stoul(response[1]);
    nextUID = stoul(response[2]);
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