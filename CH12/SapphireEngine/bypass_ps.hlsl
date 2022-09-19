struct VSOutput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

float3 CalculateAmbient(float3 normal, float3 color)
{
	float3 AmbientDown = float3(0.0f, 1.0f, 0.0f);
	float3 AmbientUp = float3(0.0f, 0.0f, 1.0f);

	float up = normal.y * 0.5f + 0.5f;
	float3 Ambient = AmbientDown + up * (AmbientUp - AmbientDown);
	return Ambient * color;
}

float4 main(VSOutput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);
	float3 color = float3(1.0f, 1.0f, 1.0f);

	return float4(CalculateAmbient(normal, color), 1.0f);
}