#pragma once
#include <string>
#include <SchmixEngine/Core/Debugging/Debug.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>
#include <SchmixEngine/Core/GameFramework/GameSubSystem.h>

namespace SchmixEngine
{
	class GameSubSystem;

	class GameState
	{
	public:
		GameState(const std::string& GameStateName = "");

		GameState(const GameState&) = delete;
		GameState& operator=(const GameState&) = delete;

		virtual ~GameState();

		void InitGameState(RenderSubSystem* Renderer);

		void ShutdownGameState();

		void UpdateGameState(float DeltaTime);

		bool IsInitialized() const;

		void SetGameSubSystem(GameSubSystem* Game);

		std::string GetGameStateName() const;

	protected:
		virtual bool Init() = 0;

		virtual bool InitRender(RenderSubSystem* Renderer) = 0;

		virtual bool Shutdown() = 0;

		virtual bool ShutdownRender(RenderSubSystem* Renderer) = 0;

		virtual void Update(float DeltaTime) = 0;

		virtual void Render(float DeltaTime, RenderSubSystem* Renderer) = 0;

		bool m_bIsInitialized;
		std::string m_GameStateName;

		GameSubSystem* m_pGameSystem;

	private:
		RenderSubSystem* m_pRenderSystem;
	};
}