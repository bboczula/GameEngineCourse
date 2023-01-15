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
	float4 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
	float4 normal : NORMAL;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	float4 position_ws = mul(input.position, world);
	float4 position_vs = mul(position_ws, view);
	output.position = position_vs;
	output.position_ws = position_ws;
	//output.normal = mul(world, input.normal);
	output.normal = mul(input.normal, world);

	return output;
}