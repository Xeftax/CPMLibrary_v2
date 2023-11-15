#ifndef CPM_HEADER
#define CPM_HEADER

#include "logmanager.h"
#include <cstdint>
#include <string>
#include <set>
#include <map>

namespace cpm {

class CpmObjectBuilder {
public:
    class Content {
    protected:
        Content(const std::map<std::string, std::string> headers);
        const std::map<std::string, std::string> headersMap;
        const std::string& getHeaderValue(const std::string& key);
        const std::string emptyString = "";
    public:
        friend CpmObjectBuilder;
        const std::string& From;
        const std::string& To;
        const std::string& Date;
        const std::string& Subject;
        const std::string& Conversation_ID;
        const std::string& Contribution_ID;
        const std::string& InReplyTo_Contribution_ID;  
        const std::string& IMDN_Message_ID;
        const std::string& Content_Type;
        const std::string& Message_Body;
        std::string format();
    };
protected:
    std::map<std::string,std::string> headers;

    struct HeaderBuilder { 
    HeaderBuilder(std::map<std::string,std::string>& headersMap);
    protected:
        std::map<std::string,std::string>& headers;
    }; 
};

class CpmFolderObjectBuilder {
public:
    class Content {
    protected:
        Content(const std::map<std::string, std::string> headers);
        const std::map<std::string, std::string> headersMap;
        const std::string& getHeaderValue(const std::string& key);
        const std::string emptyString = "";
    public:
        friend CpmFolderObjectBuilder;
        const std::string& Name;
        std::string format();
    };
protected:
    std::map<std::string,std::string> headers;

    struct HeaderBuilder { 
    HeaderBuilder(std::map<std::string,std::string>& headersMap);
    protected:
        std::map<std::string,std::string>& headers;
    };

    static CpmObjectBuilder builder;
    
};

} // namespace cpm

#endif // CPM_HEADER