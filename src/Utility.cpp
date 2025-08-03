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

    RE::FormID GetFormIDFromString(std::string formStr) 
    {
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

    int GetCWAllegiance()
    {
        const auto gAllegiance = RE::TESForm::LookupByEditorID<RE::TESGlobal>("CWPlayerAllegiance");
        if (gAllegiance) {
            return static_cast<int>(gAllegiance->value);
        }
        else {
            return 0;
        }
    }

    bool ValidateQuestCondition(Newspaper::conditionFormat condition)
    {
        const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(condition.formID);
        if (!quest) { return false; }

        const auto questStage = quest->currentStage;
        logger::info("Quest '{}' is on stage {}", quest->GetName(), questStage);

        switch (condition.op) {
        case '=': return questStage == condition.value;
        case '<': return questStage < condition.value;
        case '>': return questStage >= condition.value;
        }

        return false;
    }

}