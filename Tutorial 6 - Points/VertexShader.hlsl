cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float Size : PSIZE;
};

VertexOut main(float3 pos : POSITION, float4 color : COLOR, float size : PSIZE)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Color = color;
	vout.Size = size;
	return vout;
}