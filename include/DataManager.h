#pragma once
#include "LookupConfigs.h"

class Newspaper 
{
private:
	std::string key;
	std::string bookOLD;
	std::string bookNEW;
	std::vector<RE::BGSListForm*> formlists;
public:
	Newspaper(LookupConfigs::configFormat& configData);

};