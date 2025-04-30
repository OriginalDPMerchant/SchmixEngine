#pragma once
#include <unordered_map>
#include <memory>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/Core/GameFramework/GameState.h>

namespace SchmixEngine
{
	//Forward Declaration
	class GameState;

	class GameStateManager
	{
	public:
		GameStateManager();

		GameStateManager(const GameStateManager&) = delete;
		GameStateManager& operator=(const GameStateManager&) = delete;

		~GameStateManager();

		void SetCurrentGameState(std::string GameStateName);

		void SetCurrentGameStateToNone();

		template<typename T>
		T* CreateGameState(std::string GameStateName);

		void DestroyGameState(std::string GameStateName);

		void DestroyAllGameStates();

		GameState* GetGameState(std::string GameStateName);

		GameState* GetCurrentGameState();

		uint32_t GetNumGameStates();

		std::vector<const GameState*> GetAllGameStates();
	protected:
		std::unordered_map<std::string, std::unique_ptr<GameState>> m_GameStates;

		GameState* m_pCurrentGameState;
	};

	template<typename T>
	T* GameStateManager::CreateGameState(std::string GameStateName)
	{
		static_assert(std::is_base_of<GameState, T>(), "T must Inhertit from \"GameState\"");

		T* GameStateToReturn = nullptr;

		if (!GetGameState(GameStateName))
		{
			m_GameStates[GameStateName] = std::make_unique<T>(GameStateName);

			GameStateToReturn = static_cast<T*>(m_GameStates[GameStateName].get());
		}

		return GameStateToReturn;
	}
}