#include "Events.h"
#include "DataManager.h"
#include "Utility.h"

namespace Events
{
    CellEventHandler* CellEventHandler::GetSingleton()
    {
        static CellEventHandler singleton;
        return std::addressof(singleton);
    }

    void CellEventHandler::Register()
    {
        if (const auto player = RE::PlayerCharacter::GetSingleton()) {
            player->AsBGSActorCellEventSource()->AddEventSink<RE::BGSActorCellEvent>(GetSingleton());
            logger::info("Registered for BGSActorCellEvent");
        }
    }

    //Process player cell change event
    RE::BSEventNotifyControl CellEventHandler::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*)
    {
        if (!a_event || a_event->flags == RE::BGSActorCellEvent::CellFlag::kLeave) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->Is3DLoaded()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        //DataManager::UpdateAllEntries();
        Utility::TimeFunction("UpdateAllEntries", DataManager::UpdateAllEntries, false);

        return RE::BSEventNotifyControl::kContinue;
    }

}