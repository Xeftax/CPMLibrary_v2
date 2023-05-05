#include "cpmdialogtest.h"
#include <random>
#include <iomanip>

string TestDialogQueue::pop() { 
    string result = mQueue.front();
    mQueue.pop_front();
    return result;
}
void TestDialogQueue::add(string message) { mQueue.push_back(message); }
bool TestDialogQueue::isEmpty() { return mQueue.empty(); }


string TestRequestCoder::code(vector<string> message) {
    stringstream ss;
    for (const auto& msg : message) { ss << msg.length() << ':' << msg;}
    ss << "0:";
    return ss.str();
}
vector<string> TestRequestCoder::decode(string message) {
    vector<string> result;
    size_t i = 0;
    while (i < message.length()) {
        size_t colon_pos = message.find(':', i);
        if (colon_pos == string::npos) { return result; }
        size_t len = stoi(message.substr(i, colon_pos - i));
        i = colon_pos + 1;
        if (i + len > message.length()) { return result;}
        result.emplace_back(message.substr(i, len));
        i += len;
    }
    return result;
}

string TestUIDGenerator::getNewUID() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(0, 255);
    stringstream ss;
    for (int i = 0; i < 16; i++) {
        ss << hex << setw(2) << setfill('0') << dis(gen);
    }
    string uuid = ss.str();
    uuid.insert(8, "-");
    uuid.insert(13, "-");
    uuid.insert(18, "-");
    uuid.insert(23, "-");
    return uuid;
}
