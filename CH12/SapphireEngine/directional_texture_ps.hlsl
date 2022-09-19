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

Texture2D color : register(t0);
Texture2D depthMap : register(t1);
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

float4 main(VSOutput input) : SV_TARGET
{
	float3 DirToLight = float3(0.0f, 1.0f, 0.5f);
	float3 DirLightColor = float3(1.0f, 1.0f, 1.0f);

	float normalHeight = (float)bumpMap.Sample(sampleWrap, input.texCoord).r * 2.0f - 1.0f;
	//float3 inNormal = input.normal.xyz * normalHeight;
	float3 inNormal = PerturbNormal(input.defPosition.xyz, normalize(input.normal.xyz), normalHeight);

	float NDotL = dot(DirToLight, inNormal);
	//float3 finalColor = DirLightColor.rgb * saturate(NDotL);
	float3 baseColor = (float3)color.Sample(sampleWrap, input.texCoord);

	

	//re-homogenize position after interpolation
	input.positionLightSpace.xyz /= input.positionLightSpace.w;

	float3 ambientColor = (0.2f * baseColor);

	//if position is not visible to the light - dont illuminate it
	//results in hard light frustum
	if (input.positionLightSpace.x < -1.0f || input.positionLightSpace.x > 1.0f ||
		input.positionLightSpace.y < -1.0f || input.positionLightSpace.y > 1.0f ||
		input.positionLightSpace.z < 0.0f || input.positionLightSpace.z > 1.0f)
	{
		return float4(ambientColor.rgb, 1.0f);
	}

	//transform clip space coords to texture space coords (-1:1 to 0:1)
	input.positionLightSpace.x = input.positionLightSpace.x / 2 + 0.5;
	input.positionLightSpace.y = input.positionLightSpace.y / -2 + 0.5;

	//apply shadow map bias
	input.positionLightSpace.z -= 0.000005;

	//sample shadow map - point sampler
	float shadowMapDepth = depthMap.Sample(sampleWrap, input.positionLightSpace.xy).r;

	//if clip space z value greater than shadow map value then pixel is in shadow
	
	if (shadowMapDepth < input.positionLightSpace.z) return float4(ambientColor, 1.0f);

	float3 finalColor = (baseColor * saturate(NDotL) * 0.8f) + ambientColor;

	// Calculate if the pixel is in shadow or not
	// Calculate the projected texture coordinates.
	//float2 projectTexCoord;
	//projectTexCoord.x = input.positionLightSpace.x / input.positionLightSpace.w / 2.0f + 0.5f;
	//projectTexCoord.y = -input.positionLightSpace.y / input.positionLightSpace.w / 2.0f + 0.5f;
	//
	//float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	//if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	//{
	//	float distanceToOccluder = (float)depthMap.Sample(sampleWrap, projectTexCoord);
	//	float lightDepthValue = input.positionLightSpace.z / input.positionLightSpace.w;
	//
	//	if (lightDepthValue > distanceToOccluder)
	//	{
	//		finalColor = (baseColor * saturate(NDotL) * 0.8f) + (0.2f * baseColor);
	//	}
	//}
	//else
	//{
	//	finalColor = (baseColor * saturate(NDotL) * 0.8f) + (0.2f * baseColor);
	//}
	//{
	//	float distanceToOccluder = (float)depthMap.Sample(sampleWrap, projectTexCoord);
	//	float lightDepthValue = input.positionLightSpace.z / input.positionLightSpace.w;
	//
	//	if (lightDepthValue > distanceToOccluder)
	//	{
	//		float3 finalColor = (baseColor * saturate(NDotL) * 0.8f) + (0.2f * baseColor);
	//	}
	//	//float3 finalColor = float3(input.normal.xyz);
	//}

	return float4(finalColor.rgb, 1.0f);
	//return float4(inNormal.rgb, 1.0f);
	//return float4(normalHeight, normalHeight, normalHeight, 1.0f);
}