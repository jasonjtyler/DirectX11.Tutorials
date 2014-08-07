cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldInverse;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW: POSITION;
	float4 Color : COLOR;
	float3 NormalW : NORMAL;
};

VertexOut main(float3 pos : POSITION, float4 color : COLOR, float3 normal : NORMAL)
{
	VertexOut vout;
	vout.PosH = mul(float4(pos, 1.0f), gWorldViewProj);
	vout.PosW = mul(float4(pos, 1.0f), gWorld).xyz;
	vout.Color = color;
	vout.NormalW = mul(normal, (float3x3)gWorldInverse);
	return vout;
}