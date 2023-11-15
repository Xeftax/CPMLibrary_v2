#include "dialogexample.h"
#include <random>
#include <iomanip>
#include <sstream>

using namespace cpm;

std::string DialogQueueExample::pop() { 
    std::string result = mQueue.front();
    mQueue.pop_front();
    return result;
}
void DialogQueueExample::add(std::string message) { mQueue.push_back(message); }
bool DialogQueueExample::isEmpty() { return mQueue.empty(); }


// format : "<str1.length()>:<str1><str2.length()>:<str2>..."
std::string RequestCoderExample::code(std::vector<std::string> message) {
    std::stringstream ss;
    for (const auto& msg : message) { 
        ss << msg.length() << ':' << msg;
    }
    return ss.str();
}

std::vector<std::string> RequestCoderExample::decode(std::string message) {
    std::vector<std::string> result;
    size_t index = 0;
    while (index < message.length()) {
        size_t colon_pos = message.find(':', index);
        if (colon_pos == std::string::npos) { break;}

        size_t submessageLength = stoi(message.substr(index, colon_pos - index));
        index = colon_pos + 1;
        if (index + submessageLength > message.length()) { break;}
        
        result.push_back(message.substr(index, submessageLength));
        index += submessageLength;
    }
    return result;
}


std::string UIDGeneratorExample::getNewUID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 255);
    std::stringstream ss;
    for (int i = 0; i < 16; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << dis(gen); 
    }
    std::string uuid = ss.str();
    uuid.insert(8, "-"); 
    uuid.insert(13, "-");
    uuid.insert(18, "-");
    uuid.insert(23, "-");
    return uuid;
}

