Texture2D gDiffuseMap;

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

SamplerState SamplerLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
};

float4 main(VertexOut pin) : SV_TARGET
{
	float4 color = gDiffuseMap.Sample(SamplerLinear, pin.Tex);
	color.w = 0.5f;
	return color;
}