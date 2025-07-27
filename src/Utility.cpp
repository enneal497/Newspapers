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

    RE::FormID GetFormIDFromString(std::string formStr) {
        auto formPair = clib_util::string::split(formStr, "~");

        if (!clib_util::string::is_only_hex(formPair[0])) {
            return 0;
        }

        const auto intFormID = clib_util::string::to_num<RE::FormID>(formPair[0], true);
        const auto dataHandler = RE::TESDataHandler::GetSingleton();
        auto formID = dataHandler->LookupFormID(intFormID, formPair[1]);
        if (!formID) {
            return 0;
        }
        return formID;
    }
}