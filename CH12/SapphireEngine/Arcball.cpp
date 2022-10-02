#include "Arcball.h"
#include "Camera.h"

Sapphire::Arcball::Arcball(Camera* camera) : radius(20.0f), target(0.0f, 0.0f, 0.0f)
{
	SetCamera(camera);
	camera->position = target + DirectX::SimpleMath::Vector3(0.0f, 0.0f, radius);
	camera->forward = target - camera->position;
	camera->forward.Normalize();
}

Sapphire::Arcball::~Arcball()
{
}

void Sapphire::Arcball::MoveForward(float step)
{
	radius += 0.1;
	camera->position = target + DirectX::SimpleMath::Vector3(0.0f, 0.0f, radius);
	camera->forward = target - camera->position;
	camera->forward.Normalize();
}

void Sapphire::Arcball::MoveBackward(float step)
{
	radius -= 0.1;
	camera->position = target + DirectX::SimpleMath::Vector3(0.0f, 0.0f, radius);
	camera->forward = target - camera->position;
	camera->forward.Normalize();
}

void Sapphire::Arcball::MoveRight(float step)
{
}

void Sapphire::Arcball::MoveLeft(float step)
{
}

void Sapphire::Arcball::Rotate(float x, float y, float z)
{
	camera->rotation.x = x * 3.1415f / 180.0f;
	camera->rotation.y = y * 3.1415f / 180.0f;
	DirectX::SimpleMath::Matrix cameraRotationX = DirectX::SimpleMath::Matrix::CreateRotationX(camera->rotation.x);
	DirectX::SimpleMath::Matrix cameraRotationY = DirectX::SimpleMath::Matrix::CreateRotationY(camera->rotation.y);
	auto cameraRotation = cameraRotationX * cameraRotationY;

	// Now we really have to roatet position
	camera->position = DirectX::SimpleMath::Vector3::Transform(camera->position, cameraRotation);

	// Calculation of forward vector should be easy
	camera->forward = target - camera->position;
	camera->forward.Normalize();

	camera->right = camera->DEFAULT_UP.Cross(camera->forward);
	camera->right.Normalize();
	camera->up = camera->right.Cross(camera->forward);
	camera->up.Normalize();
	camera->up *= -1.0f;
}
