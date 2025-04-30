#include "GameStateManager.h"

namespace SchmixEngine
{
	GameStateManager::GameStateManager()
	{
		m_pCurrentGameState = nullptr;
	}

	GameStateManager::~GameStateManager()
	{
		DestroyAllGameStates();
	}

	void GameStateManager::SetCurrentGameState(std::string GameStateName)
	{
		GameState* TargetGameState = GetGameState(GameStateName);

		if (TargetGameState)
			m_pCurrentGameState = TargetGameState;
	}

	void GameStateManager::SetCurrentGameStateToNone()
	{
		m_pCurrentGameState = nullptr;
	}

	void GameStateManager::DestroyGameState(std::string GameStateName)
	{
		auto Iterator = m_GameStates.find(GameStateName);

		if (Iterator != m_GameStates.end())
		{
			if (Iterator->second.get() == m_pCurrentGameState)
				m_pCurrentGameState = nullptr;

			if (Iterator->second)
				Iterator->second->ShutdownGameState();

			m_GameStates.erase(Iterator);
		}
	}

	void GameStateManager::DestroyAllGameStates()
	{
		for (auto Iterator = m_GameStates.begin(); Iterator != m_GameStates.end(); Iterator++)
		{
			if (Iterator->second)
				Iterator->second->ShutdownGameState();
		}

		m_GameStates.clear();
		m_pCurrentGameState = nullptr;
	}

	GameState* GameStateManager::GetGameState(std::string GameStateName)
	{
		auto Iterator = m_GameStates.find(GameStateName);

		if (Iterator != m_GameStates.end())
			return Iterator->second.get();
		else
			return nullptr;
	}

	GameState* GameStateManager::GetCurrentGameState()
	{
		return m_pCurrentGameState;
	}

	uint32_t GameStateManager::GetNumGameStates()
	{
		return m_GameStates.size();
	}

	std::vector<const GameState*> GameStateManager::GetAllGameStates()
	{
		std::vector<const GameState*> GameStates;

		for (auto Iterator = m_GameStates.begin(); Iterator != m_GameStates.end(); Iterator++)
		{
			GameStates.push_back(Iterator->second.get());
		}

		return GameStates;
	}
}
