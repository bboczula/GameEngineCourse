cbuffer PerFrameData : register(b0)
{
	float4x4 view;
};

cbuffer PerObjectData : register(b1)
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
};

VSOutput main(VSInput input)
{
	VSOutput output;
	float4 position_ws = mul(input.position, world);
	float4 position_vs = mul(position_ws, view);
	//output.position = mul(position_vs, projection, mul(view, mul(world, input.position)));
	//output.position = mul(position_vs, projection);
	output.position = position_vs;
	// output.normal = float4(1.0f, 0.0f, 0.0f, 1.0f);
	// output.normal = mul(world, input.normal);
	// output.texCoord = input.texCoord;

	return output;
}