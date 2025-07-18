//API.h
#pragma once
#include <cstdint>

namespace DynamicBookFramework_API
{
	constexpr auto FrameworkPluginName = "DynamicBookFramework";
	constexpr auto InterfaceVersion = 1;

	// Define unique message types.
	enum APIMessageType : std::uint32_t
	{
		kAppendEntry = 'DBFA', // 'D' 'B' 'F' 'A' for Dynamic Book Framework Append
		kOverwriteBook = 'DBFO' // 'D' 'B' 'F' 'O' for Dynamic Book Framework Overwite
	};

	struct AppendEntryMessage
	{
		const char* bookTitleKey;   // The title of the book, which acts as the file key
		const char* textToAppend;   // The new entry text to add
	};
	
	struct OverwriteBookMessage
	{
		const char* bookTitleKey;
		const char* newContent;   // The new, permanent content for the book
	};

} // namespace DynamicBookFramework_API
