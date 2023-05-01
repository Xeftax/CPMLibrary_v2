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

vector<string> AbstractCpmCommand::Request::toStringVector() {
    return vector<string>();
}

void AbstractCpmCommand::Response::fromStringVector(vector<string> response) {
    status = static_cast<Result>(stoi(response[0]));
}

Select::Select(string& folderName) : mRequest{folderName} {}
uint Select::registrationID = CpmDialog::commandRegister<Select,string>(1);

vector<string> Select::Request::toStringVector() {
    vector<string> request = AbstractCpmCommand::Request::toStringVector();
    request.push_back(folderName);
    return request;
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
    mResponse = Response{Result::OK, 324536, 856};
}