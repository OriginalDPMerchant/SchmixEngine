#pragma once
#include <SchmixEngine/Core/Graphics/Renderer/SchmixEngineRenderSubSystemBase.h>
#include <SchmixEngine/Core/Graphics/Util/MemoryManager.h>

namespace SchmixEngine
{
	template<typename T>
	class ForwardRenderSubSystem : public SchmixEngineRenderSubSystemBase<T>
	{
	public:
		ForwardRenderSubSystem(const std::string& SubSystemName);

		ForwardRenderSubSystem(const ForwardRenderSubSystem&) = delete;
		ForwardRenderSubSystem& operator=(const ForwardRenderSubSystem&) = delete;

		virtual ~ForwardRenderSubSystem() = default;

		virtual void Render() override;

	protected:
		virtual bool InitSubSystem() override;

		//virtual bool ShutdownSubSystem() override;

		virtual void UpdateSubSystem(float DeltaTime) override;
	};
}

#include <SchmixEngine/Core/Graphics/Renderer/ForwardRenderSubSystem.inl>