#pragma once

#include "SimpleMath.h"

struct PixelDefinition
{
	PixelDefinition() : color( 0.0f, 0.0f, 0.0f, 0.0f )
	{
	}
	PixelDefinition(float r, float g, float b, float a) : color( r, g, b, a )
	{
	}
	DirectX::SimpleMath::Vector4 color;
};