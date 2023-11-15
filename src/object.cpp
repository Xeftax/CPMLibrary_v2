#include "object.h"
#include "message.h"
#include <sstream>

using namespace cpm;

// Identifier

Identifier::Identifier(const uint32_t uid, const uint32_t uidValidity, const std::string folderName) : uid(uid), uidValidity(uidValidity), folderName(folderName) {}

// AbstractMessageObject

template <typename MessageContent>
AbstractMessageObject<MessageContent>::AbstractMessageObject(const Identifier& identifier, const MessageContent& content) : identifier(identifier), content(content) {}

const std::string builder::AbstractMessageContent::emptyString = "";

builder::AbstractMessageContent::AbstractMessageContent(const std::vector<MessageHeader>& headers)
    : headers(headers),
    from(getHeaderValue("From")),
    to(getHeaderValue("To")),
    date(getHeaderValue("Date")),
    subject(getHeaderValue("Subject")),
    conversationId(getHeaderValue("Conversation-ID")),
    contributionId(getHeaderValue("Contribution-ID")),
    inReplyToContributionId(getHeaderValue("InReplyTo-Contribution-ID")),
    imdnMessageId(getHeaderValue("IMDN-Message-ID")),
    contentType(getHeaderValue("Content-Type")),
    messageBody(getHeaderValue("Message-body")) {
}

const std::string& builder::AbstractMessageContent::getHeaderValue(const std::string& key) {
    auto it = std::find_if(headers.begin(), headers.end(), [&key](const MessageHeader& header) {
        return header.key == key;
        });
    if (it != headers.end()) {
        return it->value;
    }
    return emptyString;
}

std::string builder::AbstractMessageContent::format() {
    std::stringstream textContent;
    for (const MessageHeader& header : headers) {
        if (header.key == "Message-body") continue;
        textContent << header.key << ": " << header.value << std::endl;
    }
    textContent << std::endl << messageBody;
    return textContent.str();
}

cpm::builder::HeaderBuilder::HeaderBuilder(std::vector<MessageHeader>& headers) : headers(headers) {}

// AbstractFolderObject

template <typename FolderContent>
AbstractFolderObject<FolderContent>::AbstractFolderObject(const Identifier& identifier, const FolderContent& content) : AbstractMessageObject<FolderContent>(identifier, content) {}

builder::FolderObjectContent::FolderObjectContent(const std::string& name) : name(name) {}

builder::FolderObjectContent cpm::builder::FolderObjectBuilder::name(const std::string& name) {
    return builder::FolderObjectContent{ name };
}

//Explicit instantiation
template class cpm::AbstractMessageObject<builder::FolderObjectContent>;
template class cpm::AbstractFolderObject<builder::FolderObjectContent>;
template class cpm::AbstractMessageObject<builder::MessageContent>;

