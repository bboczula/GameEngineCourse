cbuffer PerFrameData : register(b0)
{
	float4x4 view;
};

cbuffer PerFrameData2 : register(b1)
{
	float4x4 projection;
};

cbuffer PerObjectData : register(b2)
{
	float4x4 world;
}

struct VSInput
{
	float4 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	float4 position_ws = mul(input.position, world);
	float4 position_vs = mul(position_ws, view);
	output.position = position_vs;
	output.position_ws = position_ws;

	return output;
}