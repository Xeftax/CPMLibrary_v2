#ifndef CPM_CONVERSATION_HISTORY
#define CPM_GROUP_STATE

#include "object.h"

namespace cpm {

    class ConversationHistory : public AbstractFolderObject<builder::FolderObjectContent> {
    public:
        ConversationHistory(const Identifier& identifier, const builder::FolderObjectContent& content);
        using Content = builder::FolderObjectContent;
        using Builder = builder::FolderObjectBuilder;
    };

} // namespace cpm

#endif // CPM_SESSION_HISTORY