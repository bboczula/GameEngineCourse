#include "PerspectiveCamera.h"

Sapphire::PerspectiveCamera::PerspectiveCamera(float aspectRatio) : Camera(aspectRatio)
{
}

Sapphire::PerspectiveCamera::~PerspectiveCamera()
{
}

void Sapphire::PerspectiveCamera::CalculateProjectionMatrix()
{
	projection = projection.CreatePerspectiveFieldOfView(FOV_IN_RADIANS, aspectRatio, NEAR_PLANE, FAR_PLANE);
	projection = projection.Transpose();
}
