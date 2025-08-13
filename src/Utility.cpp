#include "Utility.h"
#include "DynamicBookFramework/API.h"

namespace Utility
{
    RE::FormID GetFormIDFromString(std::string formStr)
    {
        auto formPair = clib_util::string::split(formStr, "~");
        if (!clib_util::string::is_only_hex(formPair[0])) {
            logger::info("{} is not only hex", formPair[0]);
            return 0;
        }

        const auto intFormID = clib_util::string::to_num<RE::FormID>(formPair[0], true);
        const auto dataHandler = RE::TESDataHandler::GetSingleton();
        auto formID = dataHandler->LookupFormID(intFormID, formPair[1]);

        return formID ? formID : 0;
    }

    bool ReadFormID(SKSE::SerializationInterface* a_intfc, RE::FormID& formID)
    {
        a_intfc->ReadRecordData(formID);
        if (formID != 0) {
            return a_intfc->ResolveFormID(formID, formID);
        }
        return true;
    }

    int GetCWAllegiance()
    {
        static const RE::TESGlobal* gAllegiance = RE::TESForm::LookupByEditorID<RE::TESGlobal>("CWPlayerAllegiance");
        return gAllegiance ? static_cast<int>(gAllegiance->value) : 0;
    }

    bool ValidateQuestCondition(Newspaper::conditionFormat condition)
    {
        const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(condition.formID);
        if (!quest) { return false; }

        const auto questStage = quest->currentStage;
        //logger::info("Quest '{}' is on stage {}", quest->GetName(), questStage);

        switch (condition.op) {
        case '=': return questStage == condition.value;
        case '<': return questStage < condition.value;
        case '>': return questStage >= condition.value;
        }

        return false;
    }

    //Evaluate all entry conditions
    bool TestConditions(const Newspaper::conditionedEntry& cEntry)
    {
        //logger::info("Testing entry: '{}'", cEntry.formID);
        const int CWAllegiance = GetCWAllegiance();

        //Test player allegiance
        if (cEntry.playerAllegiance && cEntry.playerAllegiance.value() != CWAllegiance) {
            return false;
        }

        //Evaluate all conditions using AND
        for (const auto& condition : cEntry.questStages) {
            if (!ValidateQuestCondition(condition)) {
                return false;
            }
            //Valid condition
            //continue;
        }

        return true;
    }

    //Dispatch message to DynamicBookFramework
    //UNUSED
    void ReplaceBookContents(const RE::TESObjectBOOK* bookPtr, const std::string& bookText)
    {
        static auto* messaging = SKSE::GetMessagingInterface();
        if (!messaging) {
            // SKSE not available
            return;
        }
        logger::debug("Dispatching message to DynamicBookFramework");
        DynamicBookFramework_API::OverwriteBookMessage message;
        message.bookTitleKey = bookPtr->GetName();
        message.newContent = bookText.c_str();
        messaging->Dispatch(
            DynamicBookFramework_API::kOverwriteBook,
            &message,
            sizeof(message),
            DynamicBookFramework_API::FrameworkPluginName
        );
    }

    //Dispatch
    void PushBookDescription(const RE::TESForm* bookForm, const std::string descriptionText)
    {
        static const auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

        RE::BSFixedString descriptionString = descriptionText;

        auto args = RE::MakeFunctionArguments(
            std::move(bookForm),
            std::move(descriptionString)
        );
        RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> resultCallback;

        const auto result = vm->DispatchStaticCall("DescriptionFramework", "SetDescription", args, resultCallback);
        logger::debug("{} call to Description Framework", (result ? "Dispatched" : "Failed to dispatch"));
    }

}