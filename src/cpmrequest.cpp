#include "cpmrequest.h"
#include "cpmdialog.h"
#include <array>
#include <numeric>
#include <stdexcept>


Select::Select(string& folderName) : mRequest{0,folderName} {}
Select::registration = CpmDialog::commandRegister<Select>(mRequest.commandID);

Select::Request* Select::request() {
    return &mRequest;
}

void Select::execute() {
    mResponse = Response{Result::OK, 324536, 856};
}

Select::Response* Select::response() {
    return &mResponse;
}


