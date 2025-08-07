#pragma once
#include "Newspaper.h"

namespace Utility
{
	template<typename Func, typename... Args>
	void TimeFunction(const std::string& funcName, Func func, Args&&... args) {
		clib_util::Timer timer;
		timer.start();
		func(std::forward<Args>(args)...);
		timer.end();
		const auto runtime_us = timer.duration_us();
		logger::info("{} ran in {} microseconds", funcName, runtime_us);
		logger::info("");
	}

	template<typename itemType>
	itemType* GetFormFromString(std::string formStr) {
		auto formID = clib_util::string::split(formStr, "~");
		
		if (!clib_util::string::is_only_hex(formID[0])) {
			return nullptr;
		}

		const auto intFormID = clib_util::string::to_num<RE::FormID>(formID[0], true);
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto form = dataHandler->LookupForm(intFormID, formID[1]);
		if (!form) {
			return nullptr;
		}
		return form->As<itemType>();		
		
	}

	int GetCWAllegiance();
	bool ValidateQuestCondition(Newspaper::conditionFormat condition);
	bool TestConditions(const Newspaper::conditionedEntry& cEntry);
	RE::FormID GetFormIDFromString(std::string formStr);
	bool ReadFormID(SKSE::SerializationInterface* a_intfc, RE::FormID& formID);
	
	//UNUSED
	void ReplaceBookContents(const RE::TESObjectBOOK* bookPtr, const std::string& bookText);
}