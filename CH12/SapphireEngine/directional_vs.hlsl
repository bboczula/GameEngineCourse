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
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	float4 position_ws = mul(input.position, world);
	float4 position_vs = mul(position_ws, view);
	//output.position = mul(position_vs, projection, mul(view, mul(world, input.position)));
	output.position = mul(position_vs, projection);
	// output.normal = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.normal = mul(world, input.normal);
	output.texCoord = input.texCoord;

	return output;
}