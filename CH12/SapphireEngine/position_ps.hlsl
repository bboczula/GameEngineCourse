struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
};

float4 main(VSOutput input) : SV_TARGET
{
	return input.position_ws;
}