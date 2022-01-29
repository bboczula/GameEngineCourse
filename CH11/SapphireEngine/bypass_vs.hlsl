cbuffer PerFrameData : register(b0)
{
	float4x4 view;
};

cbuffer PerFrameData2 : register(b1)
{
	float4x4 projection;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return mul(projection, mul(view,pos));
}