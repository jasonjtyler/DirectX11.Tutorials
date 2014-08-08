cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

VertexOut main(float3 pos : POSITION, float2 tex : TEXCOORD)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.Tex = tex;
	return vout;
}