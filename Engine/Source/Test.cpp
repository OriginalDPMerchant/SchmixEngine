#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <SchmixEngine/ThirdParty/GLM/vec4.hpp>
#include <SchmixEngine/ThirdParty/GLM/mat4x4.hpp>
#include <SchmixEngine/Core/Application/Application.h>
#include <SchmixEngine/Core/Graphics/Renderer/RenderSubSystem.h>
#include <SchmixEngine/SDL2/Input/SDL2InputMappingResolver.h>
#include <SchmixEngine/Core/Debugging/ConsoleLogger.h>
#include <iostream>

class TestApp : public SchmixEngine::Application
{
protected:
	virtual bool InitApplication() override
	{
		SchmixEngine::SubSystemThread* MainThread = m_SubSystemThreadManager.GetSubSystemThread(SchmixEngine::SubSystemThreadManager::MainThreadName);

		if (MainThread)
		{
			MainThread->SetUpdateFrequency(0);
			SchmixEngine::BaseInputSubSystem* Input = MainThread->GetSubSystemManager()->CreateSubSystem<SchmixEngine::SDL2InputSubSystem>("Input");
			
			SchmixEngine::InputMapping InputMapping;
			SchmixEngine::AxisMapping Move_X =
			{
				{
					{SchmixEngine::Axis::GAMEPAD_LEFT_STICK_X, 1.0f}
				},
				{
					{SchmixEngine::Buttons::GAMEPAD_BUTTON_DPAD_RIGHT, 1.0f},
					{SchmixEngine::Buttons::GAMEPAD_BUTTON_DPAD_LEFT, -1.0f}
				}
			};

			InputMapping.SetAxisMapping("Move_X", Move_X);

			Input->SetDefaultInputMapping(InputMapping);

			MainThread->GetSubSystemManager()->CreateSubSystem<SchmixEngine::RenderSubSystem>("GL RENDER TEST");
		}

		return true;
	}

	virtual bool ShutdownApplication() override
	{
		return true;
	}
};
int main(int argc, char* argv[])
{
	SchmixEngine::ConsoleLogger ConsoleLogger;
	SchmixEngine::Logger::SetGlobalLogger(&ConsoleLogger);

	/*SchmixEngine::GraphicsAPIInitInfo InitInfo;
	InitInfo.ApplicationName = "VK_TEST";
	InitInfo.EngineName = "SchmixEngine";
	InitInfo.MaxFramesInFlight = 3;
	InitInfo.DisplayInfo.DisplayWidth = 800;
	InitInfo.DisplayInfo.DisplayHeight = 450;
	

	std::unique_ptr<SchmixEngine::GraphicsAPI> API = std::make_unique<SchmixEngine::SDL2_VulkanGraphicsAPI>();
	API->Init(InitInfo);
	API->Shutdown();*/

	TestApp App;

	App.Start();
	return 0;
}

