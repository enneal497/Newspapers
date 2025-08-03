#include "Events.h"
#include "DataManager.h"

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

        logger::info("Player changed cells");
        const auto daysPassed = RE::Calendar::GetSingleton()->GetDaysPassed();
        logger::info("daysPassed: {}", daysPassed);

        for (auto& [key, newspaper] : DataManager::newspaperMap) {
            if (newspaper.lastUpdatedDay + newspaper.updateInterval < daysPassed) {
                //Update newspaper entries
                logger::info("Updating entry for {}", key);
                newspaper.lastUpdatedDay = daysPassed;
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

}