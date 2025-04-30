#include "GameState.h"

namespace SchmixEngine
{
	GameState::GameState(const std::string& GameStateName)
	{
		m_bIsInitialized = false;
		m_GameStateName = GameStateName;
		m_pGameSystem = nullptr;
	}

	GameState::~GameState()
	{
		ShutdownGameState();
	}

	void GameState::InitGameState(RenderSubSystem* Renderer)
	{
		if (m_bIsInitialized)
		{
			SMX_DBG_LOG_WSRC("GameState Already Initialized", m_GameStateName, LogLevel::Warning);
			return;
		}

		m_pRenderSystem = Renderer;

		m_bIsInitialized = Init();
		m_bIsInitialized = m_bIsInitialized ? InitRender(m_pRenderSystem) : false;

		SMX_DBG_LOG_WSRC("Initializing GameState...", m_GameStateName, LogLevel::Debug);
	}


	void GameState::ShutdownGameState()
	{
		if (!m_bIsInitialized)
		{
			SMX_DBG_LOG_WSRC("Nothing To Shutdown As GameState Was Never Initialized", m_GameStateName, LogLevel::Warning);
			return;
		}

		m_bIsInitialized = !Shutdown();
		m_bIsInitialized = !m_bIsInitialized ? !ShutdownRender(m_pRenderSystem) : true;

		SMX_DBG_LOG_WSRC("Shutting Down GameState...", m_GameStateName, LogLevel::Debug);
	}

	void GameState::UpdateGameState(float DeltaTime)
	{
		if (!m_bIsInitialized)
		{
			SMX_DBG_LOG_WSRC("GameState Not Initialized", m_GameStateName, LogLevel::Debug);
		}
		else
		{
			Update(DeltaTime);

			if (m_pRenderSystem)
				Render(DeltaTime, m_pRenderSystem);
		}
	}

	bool GameState::IsInitialized() const
	{
		return m_bIsInitialized;
	}

	void GameState::SetGameSubSystem(GameSubSystem* Game)
	{
		m_pGameSystem = Game;
	}

	std::string GameState::GetGameStateName() const
	{
		return m_GameStateName;
	}
}