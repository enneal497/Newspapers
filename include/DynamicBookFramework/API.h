//API.h
#pragma once
#include "PCH.h" // For common headers like SKSE, RE, and standard library
#include <cstdint>

namespace DynamicBookFramework_API
{

	constexpr auto FrameworkPluginName = "DynamicBookFramework";
	constexpr auto InterfaceVersion = 1;

	// Define unique message types.
	enum APIMessageType : std::uint32_t
	{
		kAppendEntry = 'DBFA', // 'D' 'B' 'F' 'A' for Dynamic Book Framework Append
		kOverwriteBook = 'DBFO', // 'D' 'B' 'F' 'O' for Dynamic Book Framework Overwite
		kExcludeBook = 'DBFE', // 'D' 'B' 'F' 'E' for Dynamic Book Framework Exclude
		kAddDynamicText = 'DBDT', // 'D' 'B' 'D' 'T' for Dynamic Book Dynamic Text
		kRequestUIRefresh = 'DBRR', // 'D' 'B' 'R' 'R' Dynamic Book Refresh Request
		kReloadBookMappings = 'DBRL', // 
		kRequestEditorBookListRefresh = 'DBLR',
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

	struct ExcludeBookMessage {
        const char* bookTitleKey;
    };

	struct AddDynamicTextMessage {
        const char* bookTitleKey;
        const char* dynamicText;  // The text to display in the book for the current session
    };

} // namespace DynamicBookFramework_API
