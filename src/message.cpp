#include "message.h"

using namespace cpm::builder;

cpm::Message::Message(const Identifier& identifier, const builder::MessageContent& content) : AbstractMessageObject<cpm::Message::Content>(identifier, content) {}

MessageContent::MessageContent(const std::vector<MessageHeader>& headers)
    : AbstractMessageContent(headers),
    cc(getHeaderValue("CC")),
    bcc(getHeaderValue("BCC")),
    pAssertedService(getHeaderValue("P-Asserted-Service")),
    acceptContact(getHeaderValue("Accept-Contact")),
    messageCorrelator(getHeaderValue("Message-Correlator")),
    messageContext(getHeaderValue("Message-Context")) {
}

MessageBuilder::ToBuilder MessageBuilder::From(const std::string& from) {
    headers.push_back(MessageHeader{ "From", from });
    return ToBuilder{ headers };
}

MessageBuilder::CCBuilder MessageBuilder::ToBuilder::To(const std::string& to) {
    headers.push_back(MessageHeader{ "To", to });
    return CCBuilder{ headers };
}

MessageBuilder::BCCBuilder MessageBuilder::CCBuilder::CC(const std::string& cc) {
    headers.push_back(MessageHeader{ "CC", cc });
    return BCCBuilder{ headers };
}

MessageBuilder::DateBuilder MessageBuilder::BCCBuilder::BCC(const std::string& bcc) {
    headers.push_back(MessageHeader{ "BCC", bcc });
    return DateBuilder{ headers };
}

MessageBuilder::SubjectBuilder MessageBuilder::DateBuilder::Date(const std::string& date) {
    headers.push_back(MessageHeader{ "Date", date });
    return SubjectBuilder{ headers };
}

MessageBuilder::PAssertedServiceBuilder MessageBuilder::SubjectBuilder::Subject(const std::string& subject) {
    headers.push_back(MessageHeader{ "Subject", subject });
    return PAssertedServiceBuilder{ headers };
}

MessageBuilder::ConversationIDBuilder MessageBuilder::PAssertedServiceBuilder::PAssertedService(const std::string& pAssertedService) {
    headers.push_back(MessageHeader{ "P-Asserted-Service", pAssertedService });
    return ConversationIDBuilder{ headers };
}

MessageBuilder::ContributionIDBuilder MessageBuilder::ConversationIDBuilder::ConversationID(const std::string& conversationId) {
    headers.push_back(MessageHeader{ "Conversation-ID", conversationId });
    return ContributionIDBuilder{ headers };
}

MessageBuilder::InReplyToContributionIDBuilder MessageBuilder::ContributionIDBuilder::ContributionID(const std::string& contributionId) {
    headers.push_back(MessageHeader{ "Contribution-ID", contributionId });
    return InReplyToContributionIDBuilder{ headers };
}

MessageBuilder::AcceptContactBuilder MessageBuilder::InReplyToContributionIDBuilder::InReplyToContributionID(const std::string& inReplyToContributionId) {
    headers.push_back(MessageHeader{ "InReplyTo-Contribution-ID", inReplyToContributionId });
    return AcceptContactBuilder{ headers };
}

MessageBuilder::IMDNMessageIDBuilder MessageBuilder::AcceptContactBuilder::AcceptContact(const std::string& acceptContact) {
    headers.push_back(MessageHeader{ "Accept-Contact", acceptContact });
    return IMDNMessageIDBuilder{ headers };
}

MessageBuilder::MessageCorrelatorBuilder MessageBuilder::IMDNMessageIDBuilder::IMDNMessageID(const std::string& imdnMessageId) {
    headers.push_back(MessageHeader{ "IMDN-Message-ID", imdnMessageId });
    return MessageCorrelatorBuilder{ headers };
}

MessageBuilder::MessageContextBuilder MessageBuilder::MessageCorrelatorBuilder::MessageCorrelator(const std::string& messageCorrelator) {
    headers.push_back(MessageHeader{ "Message-Correlator", messageCorrelator });
    return MessageContextBuilder{ headers };
}

MessageBuilder::ContentTypeBuilder MessageBuilder::MessageContextBuilder::MessageContext(const std::string& messageContext) {
    headers.push_back(MessageHeader{ "Message-Context", messageContext });
    return ContentTypeBuilder{ headers };
}

MessageBuilder::MessageBodyBuilder MessageBuilder::ContentTypeBuilder::ContentType(const std::string& contentType) {
    headers.push_back(MessageHeader{ "Content-Type", contentType });
    return MessageBodyBuilder{ headers };
}

MessageContent MessageBuilder::MessageBodyBuilder::MessageBody(const std::string& messageBody) {
    headers.push_back(MessageHeader{ "Message-body", messageBody });
    return MessageContent(headers);
}