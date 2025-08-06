#pragma once

namespace Serialisation
{
	class Manager
	{
	private:
		mutable std::shared_mutex _lock{};

	protected:
		Manager() = default;
		~Manager() = default;

		Manager(const Manager&) = delete;
		Manager(Manager&&) = delete;

		Manager& operator=(const Manager&) = delete;
		Manager& operator=(Manager&&) = delete;

	public:
		static Manager* GetSingleton() {
			static Manager singleton;
			return std::addressof(singleton);
		}

		void OnGameSaved(SKSE::SerializationInterface* a_intfc);
		void OnGameLoaded(SKSE::SerializationInterface* a_intfc);
		void OnRevert(SKSE::SerializationInterface* a_intfc);
	};

	const unsigned long sUID = _byteswap_ulong('SNWS');
	const unsigned long sEntries = _byteswap_ulong('ENTR');
	const unsigned long sVersion = 1;

	void InitialiseSerialisation();
	void SaveCallback(SKSE::SerializationInterface* a_intfc);
	void LoadCallback(SKSE::SerializationInterface* a_intfc);
	void RevertCallback(SKSE::SerializationInterface* a_intfc);

};