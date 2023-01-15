struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
	float4 normal : NORMAL;
};

struct PSOutput
{
	float4 position_ws : SV_Target0;
	float4 normal : SV_Target1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	output.position_ws = input.position_ws;
	output.normal = float4(input.normal.rgb, 1.0f);
	return output;
}