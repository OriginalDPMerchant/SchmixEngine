#include "Camera.h"

namespace SchmixEngine
{
	Camera::Camera()
		: m_ViewMatrix(glm::mat4(1.0f)),
		m_ProjectionMatrix(glm::mat4(1.0f)),
		m_Position(glm::vec3(0.0f)),
		m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		m_FOV(60.0f),
		m_Width(1280.0f),
		m_Height(720.0f),
		m_NearClipPlane(-500.0f),
		m_FarClipPlane(500.0f),
		m_ProjectionType(CameraProjectionType::Orthographic)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::UpdateViewMatrix()
	{
		// Create a rotation matrix from the quaternion
		glm::mat4 RotationMatrix = glm::mat4_cast(m_Rotation);

		// Create a translation matrix for the camera position
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), -m_Position);

		// Combine the rotation and translation matrices to get the view matrix
		m_ViewMatrix = RotationMatrix * TranslationMatrix;
	}

	void Camera::UpdateProjectionMatrix()
	{
		//sets m_ProjectionMatrix to an orthographic or a perspective projection matrix based on the m_ProjectionType
		switch (m_ProjectionType)
		{
		case CameraProjectionType::Orthographic:
			m_ProjectionMatrix = glm::ortho(-m_Width / 2.0f, m_Width / 2.0f, -m_Height / 2.0f, m_Height / 2.0f, m_NearClipPlane, m_FarClipPlane);
			break;
		case CameraProjectionType::Perspective:
		{
			float AspectRatio = m_Width / m_Height;

			m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), AspectRatio, m_NearClipPlane, m_FarClipPlane);
		}
		break;
		default:
			break;
		}
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	glm::mat4 Camera::GetProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}

	void Camera::SetPosition(const glm::vec3& Position)
	{
		m_Position = Position;
	}

	glm::vec3 Camera::GetPosition()
	{
		return m_Position;
	}

	void Camera::SetRotation(const glm::quat& Rotation)
	{
		m_Rotation = Rotation;
	}

	glm::quat Camera::GetRotation()
	{
		return m_Rotation;
	}

	void Camera::SetWidth(float Width)
	{
		m_Width = Width;
	}

	float Camera::GetWidth()
	{
		return m_Width;
	}

	void Camera::SetHeight(float Height)
	{
		m_Height = Height;
	}

	float Camera::GetHeight()
	{
		return m_Height;
	}

	void Camera::SetNearClipPlane(float NearClipPlane)
	{
		m_NearClipPlane = NearClipPlane;
	}

	float Camera::GetNearClipPlane()
	{
		return m_NearClipPlane;
	}

	void Camera::SetFarClipPlane(float FarClipPlane)
	{
		m_FarClipPlane = FarClipPlane;
	}

	float Camera::GetFarClipPlane()
	{
		return m_FarClipPlane;
	}

	void Camera::SetFOV(float FOV)
	{
		m_FOV = FOV;
	}

	float Camera::GetFOV()
	{
		return m_FOV;
	}

	void Camera::SetProjectionType(CameraProjectionType ProjectionType)
	{
		m_ProjectionType = ProjectionType;
	}

	CameraProjectionType Camera::GetProjectionType()
	{
		return m_ProjectionType;
	}
}
