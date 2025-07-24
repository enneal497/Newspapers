#pragma once

namespace Utility
{
	void ReplaceBookContents(const RE::TESObjectBOOK* bookPtr, const std::string& bookText);

	template<typename Func, typename... Args>
	void TimeFunction(const std::string& funcName, Func func, Args&&... args) {
		clib_util::Timer timer;
		timer.start();
		func(std::forward<Args>(args)...);
		timer.end();
		const auto runtime_us = timer.duration_us();
		logger::info("{} ran in {} microseconds", funcName, runtime_us);
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
}