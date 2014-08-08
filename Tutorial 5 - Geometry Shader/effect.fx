cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

RasterizerState SolidRS
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};

VertexOut VS( float3 pos : POSITION , float4 color : COLOR)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Color = color;
	return vout;
}

float4 PS( VertexOut pin) : SV_TARGET
{
	return pin.Color;
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