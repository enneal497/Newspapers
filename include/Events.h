#pragma once

namespace Events
{
    class CellEventHandler:
        public RE::BSTEventSink<RE::BGSActorCellEvent>
    {
    public:
        static CellEventHandler* GetSingleton();
        static void Register();

        RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) override;

    private:
        CellEventHandler() = default;
        CellEventHandler(const CellEventHandler&) = delete;
        CellEventHandler(CellEventHandler&&) = delete;

        ~CellEventHandler() override = default;

        CellEventHandler& operator=(const CellEventHandler&) = delete;
        CellEventHandler& operator=(CellEventHandler&&) = delete;

    };
}