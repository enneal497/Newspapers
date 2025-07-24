#pragma once

namespace Serialisation
{
	const unsigned long sUID = _byteswap_ulong('SNWS');
	const unsigned long sConfigs = _byteswap_ulong('CNFG');
	const unsigned long sVersion = 1;

	void InitialiseSerialisation();
	static void OnGameSaved(SKSE::SerializationInterface* serialisation);
	static void OnGameLoaded(SKSE::SerializationInterface* serialisation);
	static void OnRevert(SKSE::SerializationInterface* serialisation);
};