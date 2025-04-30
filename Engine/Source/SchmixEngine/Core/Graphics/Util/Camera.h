#pragma once
#include <SchmixEngine/ThirdParty/glm/glm.hpp>
#include <SchmixEngine/ThirdParty/glm/gtc/quaternion.hpp>

namespace SchmixEngine
{
	enum class CameraProjectionType : uint32_t
	{
		Orthographic = 0,
		Perspective = 1
	};

	class Camera
	{
	public:
		Camera();
		virtual ~Camera();

		//Re-calculates the ViewMatrix
		void UpdateViewMatrix();

		//Re-calculates the Projection Matrix
		void UpdateProjectionMatrix();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		void SetPosition(const glm::vec3& Position);
		glm::vec3 GetPosition();

		void SetRotation(const glm::quat& Rotation);
		glm::quat GetRotation();

		void SetWidth(float Width);
		float GetWidth();

		void SetHeight(float Height);
		float GetHeight();

		void SetNearClipPlane(float NearClipPlane);
		float GetNearClipPlane();

		void SetFarClipPlane(float FarClipPlane);
		float GetFarClipPlane();

		void SetFOV(float FOV);
		float GetFOV();

		void SetProjectionType(CameraProjectionType ProjectionType);
		CameraProjectionType GetProjectionType();

	protected:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		glm::vec3 m_Position;

		glm::quat m_Rotation;

		float m_Width;
		float m_Height;

		float m_NearClipPlane;
		float m_FarClipPlane;

		float m_FOV;

		CameraProjectionType m_ProjectionType;
	};
}