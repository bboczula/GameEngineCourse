struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD1;
};

Texture2D color : register(t0);
SamplerState sampleWrap : register(s0);

struct PSOutput
{
	float4 position_ws : SV_Target0;
	float4 normal : SV_Target1;
	float4 albedo : SV_Target2;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	output.position_ws = input.position_ws;
	output.normal = float4(input.normal.rgb, 1.0f);
	output.albedo = color.Sample(sampleWrap, input.texCoord);
	return output;
}