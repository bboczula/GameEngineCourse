#pragma once

#include "SimpleMath.h"
#include "Logger.h"

namespace Sapphire
{
	class Camera
	{
	public:
		Camera(float aspectRatio);
		~Camera();
		DirectX::SimpleMath::Matrix* GetViewMatrixPtr();
		DirectX::SimpleMath::Matrix* GetProjectionMatrixPtr();
		DirectX::SimpleMath::Matrix* GetViewProjectionMatrixPtr();
		void SetPosition(DirectX::SimpleMath::Vector3 position);
		void SetTarget(DirectX::SimpleMath::Vector3 target);
		void SetUp(DirectX::SimpleMath::Vector3 up);
		void LogInfo();
	protected:
		void RecalculateBasicVectors(DirectX::SimpleMath::Matrix& cameraRotation);
		virtual void CalculateProjectionMatrix();
		void CalculateViewMatrix();
		virtual void CalculateViewProjectionMatrix();
		const float NEAR_PLANE = 0.01f;
		const float FAR_PLANE = 1000.0f;
		const float FOV_IN_RADIANS = DirectX::XMConvertToRadians(36.0f);
		const DirectX::SimpleMath::Vector3 DEFAULT_UP = { 0.0f, 1.0f, 0.0f };
		const DirectX::SimpleMath::Vector3 DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f };
		const DirectX::SimpleMath::Vector3 DEFAULT_RIGTH = { 1.0f, 0.0f, 0.0f };
		float aspectRatio;
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 target;
		DirectX::SimpleMath::Vector3 up;
		DirectX::SimpleMath::Vector3 right;
		DirectX::SimpleMath::Vector3 pitchYawRoll;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		DirectX::SimpleMath::Matrix viewProjection;
	};
}