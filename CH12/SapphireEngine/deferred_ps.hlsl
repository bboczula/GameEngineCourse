struct VSOutput
{
	float4 position : SV_POSITION;
	float4 position_ws : TEXCOORD;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 defPosition : TEXCOORD2;
	float2 texCoord : TEXCOORD1;
};

Texture2D color : register(t0);
Texture2D bumpMap : register(t2);
SamplerState sampleWrap : register(s0);

float3 PerturbNormal(float3 surf_pos, float3 surf_norm, float height)
{
	float3 vSigmaS = ddx(surf_pos);
	float3 vSigmaT = ddy(surf_pos);
	float3 vN = surf_norm; // normalized
	float3 vR1 = cross(vSigmaT, vN);
	float3 vR2 = cross(vN, vSigmaS);
	float fDet = dot(vSigmaS, vR1);
	float dBs = ddx_fine(height * 0.025);
	float dBt = ddy_fine(height * 0.025);
	float3 vSurfGrad = sign(fDet) * (dBs * vR1 + dBt * vR2);
	return normalize(abs(fDet) * vN - vSurfGrad);

	// float3 P_prime = surf_pos + height * surf_norm;
	// float3 vSigmaT = ddy_fine(P_prime);
	// float3 vSigmaS = ddx_fine(P_prime);

	//float3 S = float3(1.0f, 0.0f, ddx(height));
	//float3 T = float3(0.0f, 1.0f, ddy(height));

	//float3 normal = normalize(cross(vSigmaT, vSigmaS));
	//float3 normal = normalize(cross(S, T));
	//return normal;
	//return float3(-1.0, -1.0, 0.0);
}

struct PSOutput
{
	float4 position_ws : SV_Target0;
	float4 normal : SV_Target1;
	float4 albedo : SV_Target2;
};

PSOutput main(VSOutput input)
{
	float normalHeight = (float)bumpMap.Sample(sampleWrap, input.texCoord).r * 2.0f - 1.0f;
#ifdef USE_BUMP_MAP
	float3 inNormal = PerturbNormal(input.defPosition.xyz, normalize(input.normal.xyz), normalHeight);
#else
	float3 inNormal = normalize(input.normal.xyz);
#endif // USE_BUMP_MAP

	PSOutput output;
	output.position_ws = input.position_ws;
	output.normal = float4(inNormal.xyz, 1.0f);
	output.albedo = color.Sample(sampleWrap, input.texCoord);
	return output;
}