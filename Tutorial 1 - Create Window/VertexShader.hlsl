cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

/*
RasterizerState SolidRS
{
	FillMode = Solid;
	CullMode = Back;
	FrontCounterClockwise = false;
};
*/

VertexOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Color = color;
	return vout;
}