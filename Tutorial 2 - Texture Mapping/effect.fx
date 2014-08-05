cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

RasterizerState SolidRS
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};

SamplerState SamplerLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D gDiffuseMap;

VertexOut VS( float3 pos : POSITION , float2 tex : TEXCOORD)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Tex = tex;
	return vout;
}

float4 PS( VertexOut pin) : SV_TARGET
{
	return gDiffuseMap.Sample(SamplerLinear, pin.Tex);
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

technique11 TechColor
{
	pass P0
	{
		SetVertexShader(CompileShader (vs_4_0, VS()));
		SetPixelShader(CompileShader (ps_4_0, PS()));

		SetRasterizerState(SolidRS);
	}
};