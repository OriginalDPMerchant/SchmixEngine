#include "GameSubSystem.h"

namespace SchmixEngine
{
	GameSubSystem::GameSubSystem(const std::string& Name)
		: SubSystem(Name),
		m_pInputSubSystem(nullptr),
		m_pRenderSubSystem(nullptr),
		m_pGameStateManager(std::make_unique<GameStateManager>())
	{
	}

	bool GameSubSystem::InitSubSystem()
	{
		return true;
	}

	bool GameSubSystem::ShutdownSubSystem()
	{
		m_pGameStateManager->DestroyAllGameStates();
		return true;
	}

	void GameSubSystem::UpdateSubSystem(float DeltaTime)
	{
		GameState* CurrentGameState = m_pGameStateManager->GetCurrentGameState();

		if (CurrentGameState)
		{
			if (!CurrentGameState->IsInitialized())
				CurrentGameState->InitGameState(m_pRenderSubSystem);

			CurrentGameState->SetGameSubSystem(this);
			CurrentGameState->UpdateGameState(DeltaTime);
		}
	}

	BaseInputSubSystem* GameSubSystem::GetInputSubSystem()
	{
		return m_pInputSubSystem;
	}

	void GameSubSystem::SetInputSystem(BaseInputSubSystem* Input)
	{
		m_pInputSubSystem = Input;
	}

	RenderSubSystem* GameSubSystem::GetRenderSubSystem()
	{
		return m_pRenderSubSystem;
	}

	void GameSubSystem::SetRenderSubSystem(RenderSubSystem* Renderer)
	{
		m_pRenderSubSystem = Renderer;
	}

	GameStateManager* GameSubSystem::GetGameStateManager()
	{
		return m_pGameStateManager.get();
	}
}