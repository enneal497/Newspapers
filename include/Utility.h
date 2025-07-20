#pragma once

namespace Utility
{
	void ReplaceBookContents(const std::string& bookTitle, const std::string& bookText);

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
	itemType* GetFormFromID(std::string strID) {
		//TODO: Add support for FormID~ModName.esp
		auto form = RE::TESForm::LookupByEditorID<itemType>(strID);
		return form;
	}
}