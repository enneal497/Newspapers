#include "Utility.h"
#include "DynamicBookFramework/API.h"

namespace Utility
{
    void ReplaceBookContents(const RE::TESObjectBOOK* bookPtr, const std::string& bookText)
    {
        auto* messaging = SKSE::GetMessagingInterface();
        if (!messaging) {
            // SKSE not available
            return;
        }
        logger::debug("Dispatching message to DynamicBookFramework");
        DynamicBookFramework_API::OverwriteBookMessage message;
        //TODO - GetName() is untested, may not work as expected
        message.bookTitleKey = bookPtr->GetName();
        message.newContent = bookText.c_str();
        messaging->Dispatch(
            DynamicBookFramework_API::kOverwriteBook,
            &message,
            sizeof(message),
            DynamicBookFramework_API::FrameworkPluginName
        );
    }
}