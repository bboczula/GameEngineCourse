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
	float4 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 positionLightSpace : TEXCOORD1;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 defPosition : TEXCOORD2;
	float2 texCoord : TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	float4 position_ws = mul(input.position, world);
	float4 position_vs = mul(position_ws, view);
	float4 position_vs_light = mul(position_ws, projection);
	//output.position = mul(position_vs, projection, mul(view, mul(world, input.position)));
	//output.position = mul(position_vs, projection);
	output.position = position_vs;
	output.positionLightSpace = position_vs_light;
	// output.normal = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.normal = mul(world, input.normal);
	output.texCoord = float2(input.texCoord.x, -1.0f * input.texCoord.y);
	//output.texCoord = float2(0.0f, 0.0f);

	// We have to calculate the bi-tangent
	float3 binormal = cross(input.normal, input.tangent);

	// Calculate the TBN matrix
	output.tangent = mul(input.tangent, world);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(binormal, world);
	output.binormal = normalize(output.binormal);

	output.defPosition = position_ws;

	return output;
}