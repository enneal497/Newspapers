#pragma once

namespace Serialisation 
{
	const auto sUID = _byteswap_ulong('SNWS');
	const auto sConfigs = _byteswap_ulong('CNFG');
	const auto sVersion = 1;

	void InitialiseSerialisation();
	void OnGameSaved(SKSE::SerializationInterface* serialisation);
	void OnGameLoaded(SKSE::SerializationInterface* serialisation);
	void OnRevert(SKSE::SerializationInterface* serialisation);
}