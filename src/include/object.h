#ifndef CPM_OBJECT
#define CPM_OBJECT

#include <map>
#include <string>
#include <memory>
#include <functional>

namespace cpm
{
    class MessageHeader;

    namespace builder
    {

        class AbstractMessageContent {
        protected:
            friend class AbstractMessageObject;
            AbstractMessageContent(const std::vector<MessageHeader>& headers);
            const std::vector<MessageHeader> headers; // needs to be defined before the other members
            const std::string& getHeaderValue(const std::string& key);
            static const std::string emptyString;
        public:
            const std::string& from;
            const std::string& to;
            const std::string& date;
            const std::string& subject;
            const std::string& conversationId;
            const std::string& contributionId;
            const std::string& inReplyToContributionId;
            const std::string& imdnMessageId;
            const std::string& contentType;
            const std::string& messageBody;
            std::string format();

        };

        class FolderObjectContent {
        public:
            const std::string name;

        protected:
            friend class AbstractFolderObject;
            friend class FolderObjectBuilder; // ???????????
            FolderObjectContent(const std::string& name);
        };

        struct FolderObjectBuilder {
            FolderObjectContent name(const std::string& name);
        };

        struct HeaderBuilder {
            HeaderBuilder(std::vector<MessageHeader>& headers);
        protected:
            std::vector<MessageHeader>& headers;
        };

    } // namespace builder

    struct MessageHeader {
        const std::string key;
        const std::string value;
    };

    struct Identifier {
        const uint32_t uid;
        const uint32_t uidValidity;
        const std::string folderName;
        Identifier(const uint32_t uid, const uint32_t uidValidity, const std::string folderName); //temporary

    private:
        friend class StorageUnit;
        //Identifier(const uint32_t uid, const uint32_t uidValidity, const std::string folderName);
    };

    template <typename MessageContent>
    class AbstractMessageObject {
    public:
        AbstractMessageObject(const Identifier& identifier, const MessageContent& content);
        using Content = MessageContent;
        using Builder = builder::HeaderBuilder;
        const Identifier identifier;
        const MessageContent content;

    protected:
        friend class StorageUnit;
        friend class CpmConversationHistoryTestSuite; //temporary
        //AbstractMessageObject(const Identifier& identifier, const Content& content);
    };

    template <typename FolderContent>
    class AbstractFolderObject : public AbstractMessageObject<FolderContent> {
    public:
        AbstractFolderObject(const Identifier& identifier, const FolderContent& content);
        using Content = FolderContent;
        using Builder = builder::FolderObjectBuilder;

        template <typename CpmObject>
        std::shared_ptr<CpmObject> append(std::function<typename CpmObject::Content(typename CpmObject::Builder)> builderFunc) {
            auto builder = typename CpmObject::Builder();
            auto content = builderFunc(builder);
            return std::make_shared<CpmObject>(Identifier(0, 0, this->identifier.folderName), content);
        }

        template <typename CpmObject>
        std::shared_ptr<CpmObject> fetch(const uint32_t uid) {
            return nullptr;
        }
    protected:
        //AbstractFolderObject(const Identifier& identifier, const Content& content);
    };

} // namespace cpm

#endif // CPM_OBJECT