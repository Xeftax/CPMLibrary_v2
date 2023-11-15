#include "headers.h"
#include <sstream>

using namespace cpm;

CpmObjectBuilder::Content::Content(const std::map<std::string, std::string> headers) 
  : headersMap(headers),
    From(getHeaderValue("From")), 
    To(getHeaderValue("To")), 
    Date(getHeaderValue("Date")), 
    Subject(getHeaderValue("Subject")), 
    Conversation_ID(getHeaderValue("Conversation-ID")),
    Contribution_ID(getHeaderValue("Contribution-ID")),
    InReplyTo_Contribution_ID(getHeaderValue("InReplyTo-Contribution-ID")),
    IMDN_Message_ID(getHeaderValue("IMDN-Message-ID")),
    Content_Type(getHeaderValue("Content-Type")),
    Message_Body(getHeaderValue("Message-body")) {
}

const std::string& CpmObjectBuilder::Content::getHeaderValue(const std::string& key) {
    auto it = headersMap.find(key);
    if (it != headersMap.end()) {
        return it->second;
    }
    return emptyString;
}

std::string CpmObjectBuilder::Content::format() {
    std::stringstream text;
    std::map<std::string, std::string> headers_wo_message_body = headersMap;
    headers_wo_message_body.erase("Message-body");
    for (auto it = headersMap.begin(); it != headersMap.end(); ++it) {
        text << it->first << ": " << it->second << std::endl;
    }
    text << std::endl << Message_Body;
    return text.str();

}

CpmObjectBuilder::HeaderBuilder::HeaderBuilder(std::map<std::string, std::string> &headersMap) : headers(headersMap) {}

