Texture2D gTexture;
SamplerState gSamplerState;

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

float4 main(VertexOut v) : SV_TARGET
{
	float4 result = v.Color;
	result.w = gTexture.Sample(gSamplerState, v.Tex).w;

	return result;

}