#ifndef CPM_GROUPE_STATE
#define CPM_GROUPE_STATE

#include "logmanager.h"
#include "headers.h"
#include <cstdint>
#include <string>
#include <set>
#include <map>

namespace cpm {

class GroupStateBuilder : public CpmObjectBuilder{
public: 
    class Content : public CpmObjectBuilder::Content{
    Content(const std::map<std::string, std::string> headers);
    public:
        friend GroupStateBuilder; 
    };
private:
    struct Message_BodyBuilder : HeaderBuilder {
        Content Message_Body(const std::string& message_body);
    };
    struct Content_TypeBuilder : HeaderBuilder {
        Message_BodyBuilder Content_Type(const std::string& content_type);
    };
    struct IMDN_Message_IDBuilder : HeaderBuilder {
        Content_TypeBuilder IMDN_Message_ID(const std::string& imdn_message_id);
    };
    struct InReplyTo_Contribution_IDBuilder : IMDN_Message_IDBuilder {
        IMDN_Message_IDBuilder InReplyTo_Contribution_ID(const std::string& in_reply_to_contribution_id);
    };
    struct Contribution_IDBuilder : HeaderBuilder {
        InReplyTo_Contribution_IDBuilder Contribution_ID(const std::string& contribution_id);
    };
    struct Conversation_IDBuilder : HeaderBuilder {
        Contribution_IDBuilder Conversation_ID(const std::string& conversation_id);
    };
    struct SubjectBuilder : Conversation_IDBuilder {
        Conversation_IDBuilder Subject(const std::string& subject);
    };
    struct DateBuilder : HeaderBuilder {
        SubjectBuilder Date(const std::string& date);
    };
    struct ToBuilder : HeaderBuilder {
        DateBuilder To(const std::string& to);
    };

public:
    ToBuilder From(const std::string& from);
    
};

} // namespace cpm

#endif // CPM_GROUPE_STATE