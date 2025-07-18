#include "Utility.h"
#include "DynamicBookFramework/API.h"

namespace Utility
{
    void ReplaceBookContents(const std::string& bookTitle, const std::string& bookText)
    {
        logger::info("Attempting to modify contents");
        auto* messaging = SKSE::GetMessagingInterface();
        if (!messaging) {
            // SKSE not available
            return;
        }
        DynamicBookFramework_API::OverwriteBookMessage message;
        message.bookTitleKey = bookTitle.c_str();
        message.newContent = bookText.c_str();
        messaging->Dispatch(
            DynamicBookFramework_API::kOverwriteBook,
            &message,
            sizeof(message),
            DynamicBookFramework_API::FrameworkPluginName
        );
    }
}