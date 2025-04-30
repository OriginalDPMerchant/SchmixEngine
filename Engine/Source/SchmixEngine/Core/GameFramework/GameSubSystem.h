#pragma once
#include <SchmixEngine/Core/Application/SubSystem.h>
#include <SchmixEngine/Core/Input/BaseInputSubSystem.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>
#include <SchmixEngine/Core/GameFramework/GameStateManager.h>

namespace SchmixEngine
{
	class GameStateManager;

	class GameSubSystem : public SubSystem
	{
	public:
		GameSubSystem(const std::string& Name);

		GameSubSystem(const GameSubSystem&) = delete;
		GameSubSystem operator=(const GameSubSystem&) = delete;

		virtual ~GameSubSystem() = default;

		virtual bool InitSubSystem() override;
		virtual bool ShutdownSubSystem() override;

		virtual void UpdateSubSystem(float DeltaTime) override;

		BaseInputSubSystem* GetInputSubSystem();
		void SetInputSystem(BaseInputSubSystem* Input);

		RenderSubSystem* GetRenderSubSystem();
		void SetRenderSubSystem(RenderSubSystem* Renderer);

		GameStateManager* GetGameStateManager();

	protected:
		BaseInputSubSystem* m_pInputSubSystem;
		RenderSubSystem* m_pRenderSubSystem;

		std::unique_ptr<GameStateManager> m_pGameStateManager;
	};
}