#include "Camera.h"

Sapphire::Camera::Camera(float aspectRatio)
	: position(0.0f, 2.0f, 0.0f), target(DEFAULT_FORWARD), up(DEFAULT_UP), right(DEFAULT_RIGTH),
	pitchYawRoll(0.0f, 0.0f, 0.0f), aspectRatio(aspectRatio)
{
	LogInfo();
}

Sapphire::Camera::~Camera()
{
}

DirectX::SimpleMath::Matrix* Sapphire::Camera::GetViewMatrixPtr()
{
	CalculateViewMatrix();
	return &view;
}

DirectX::SimpleMath::Matrix* Sapphire::Camera::GetProjectionMatrixPtr()
{
	CalculateProjectionMatrix();
	return &projection;
}

DirectX::SimpleMath::Matrix* Sapphire::Camera::GetViewProjectionMatrixPtr()
{
	CalculateViewProjectionMatrix();
	return &viewProjection;
}

void Sapphire::Camera::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	this->position = position;
	DirectX::SimpleMath::Matrix identity = DirectX::SimpleMath::Matrix::Identity;
	LogInfo();
}

void Sapphire::Camera::SetTarget(DirectX::SimpleMath::Vector3 target)
{
	// Maybe if i "rotate" the camera, I need to recalculate up and right?
	this->target = target;
	LogInfo();
}

void Sapphire::Camera::SetUp(DirectX::SimpleMath::Vector3 up)
{
	this->up = up;
}

void Sapphire::Camera::LogInfo()
{
	Sapphire::Logger::GetInstance().Log("CAMERA:\n");
	Sapphire::Logger::GetInstance().Log("Position: %f %f %f\n", position.x, position.y, position.z);
	Sapphire::Logger::GetInstance().Log("Target: %f %f %f\n", target.x, target.y, target.z);
	Sapphire::Logger::GetInstance().Log("Up: %f %f %f\n", up.x, up.y, up.z);
	Sapphire::Logger::GetInstance().Log("Right: %f %f %f\n", right.x, right.y, right.z);
}

void Sapphire::Camera::CalculateProjectionMatrix()
{
	projection = projection.CreatePerspectiveFieldOfView(FOV_IN_RADIANS, aspectRatio, NEAR_PLANE, FAR_PLANE);
	projection = projection.Transpose();
}

void Sapphire::Camera::CalculateViewMatrix()
{
	// If you want to get a correct result, the UP vector has to be correct.
	// So if you look down, then Up has to be (0, 0, 1) instaed of (0, 1, 0).
	view = view.CreateLookAt(position, target, up);
	view = view.Transpose();
}

void Sapphire::Camera::CalculateViewProjectionMatrix()
{
	view = view.CreateLookAt(position, target, up);
	projection = projection.CreatePerspectiveFieldOfView(FOV_IN_RADIANS, aspectRatio, NEAR_PLANE, FAR_PLANE);
	viewProjection = view * projection;
	viewProjection = viewProjection.Transpose();
}

void Sapphire::Camera::RecalculateBasicVectors(DirectX::SimpleMath::Matrix& cameraRotation)
{
	// Recalculate target vector
	target = DirectX::SimpleMath::Vector3::Transform(DEFAULT_FORWARD, cameraRotation);
	target.Normalize();

	// This is characteristic for FPP cameras, this should somehow be in upper class
	target += position;

	// Recalcualte right vector
	right = DirectX::SimpleMath::Vector3::Transform(DEFAULT_RIGTH, cameraRotation);
	right.Normalize();

	// Recalculate up vector
	up = DirectX::SimpleMath::Vector3::Transform(DEFAULT_UP, cameraRotation);
	up.Normalize();
}