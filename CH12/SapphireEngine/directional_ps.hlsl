struct VSOutput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float4 main(VSOutput input) : SV_TARGET
{
	float3 DirToLight = float3(1.0f, 0.0f, 1.0f);
	float3 DirLightColor = float3(1.0f, 1.0f, 1.0f);

	float NDotL = dot(DirToLight, input.normal.xyz);
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);
	//float3 finalColor = float3(input.normal.xyz);

	return float4(finalColor.rgb, 1.0f);
}