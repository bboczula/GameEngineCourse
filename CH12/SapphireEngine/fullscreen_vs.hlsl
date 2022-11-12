struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VSOutput main(uint vertexID : SV_VertexID)
{
	VSOutput output;

	// Generate clip space position
	output.position.x = (float)(vertexID / 2) * 4.0f - 1.0f;
	output.position.y = (float)(vertexID % 2) * 4.0f - 1.0f;
	output.position.z = 0.0f;
	output.position.w = 1.0f;
	
	return output;
}