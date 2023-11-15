#include "groupstate.h"
#include <sstream>

using namespace cpm;

GroupStateBuilder::Content::Content(const std::map<std::string, std::string> headers) : CpmObjectBuilder::Content(headers) {}

GroupStateBuilder::ToBuilder GroupStateBuilder::From(const std::string& from) {
    headers["From"] = from;
    return ToBuilder{headers};
}

GroupStateBuilder::DateBuilder GroupStateBuilder::ToBuilder::To(const std::string& to) {
    headers["To"] = to;
    return DateBuilder{headers};
}

GroupStateBuilder::SubjectBuilder GroupStateBuilder::DateBuilder::Date(const std::string& date) {
    headers["Date"] = date;
    return SubjectBuilder{headers};
}

GroupStateBuilder::Conversation_IDBuilder GroupStateBuilder::SubjectBuilder::Subject(const std::string& subject) {
    headers["Subject"] = subject;
    return Conversation_IDBuilder{headers};
}

GroupStateBuilder::Contribution_IDBuilder GroupStateBuilder::Conversation_IDBuilder::Conversation_ID(const std::string& conversation_id) {
    headers["Conversation-ID"] = conversation_id;
    return Contribution_IDBuilder{headers};
}

GroupStateBuilder::InReplyTo_Contribution_IDBuilder GroupStateBuilder::Contribution_IDBuilder::Contribution_ID(const std::string& contribution_ID) {
    headers["Contribution-ID"] = contribution_ID;
    return InReplyTo_Contribution_IDBuilder{headers};
}

GroupStateBuilder::IMDN_Message_IDBuilder GroupStateBuilder::InReplyTo_Contribution_IDBuilder::InReplyTo_Contribution_ID(const std::string& in_reply_to_contribution_id) {
    headers["InReplyTo-Contribution-ID"] = in_reply_to_contribution_id;
    return IMDN_Message_IDBuilder{headers};
}

GroupStateBuilder::Content_TypeBuilder GroupStateBuilder::IMDN_Message_IDBuilder::IMDN_Message_ID(const std::string& imdn_message_id) {
    headers["IMDN-Message-ID"] = imdn_message_id;
    return Content_TypeBuilder{headers};
}

GroupStateBuilder::Message_BodyBuilder GroupStateBuilder::Content_TypeBuilder::Content_Type(const std::string& content_type) {
    headers["Content-Type"] = content_type;
    return Message_BodyBuilder{headers};
}

GroupStateBuilder::Content GroupStateBuilder::Message_BodyBuilder::Message_Body(const std::string& message_body) {
    headers["Message-body"] = message_body;
    return Content(headers);
}

