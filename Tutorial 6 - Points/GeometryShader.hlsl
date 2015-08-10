struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float Size : PSIZE;
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

cbuffer cbRare
{
	float2 screenDimensions;
	float2 pad;
};

[maxvertexcount(4)]
void main(point VertexOut input[1], inout TriangleStream<GSOutput> output)
{
	float4 p0 = input[0].PosH;
	float size = input[0].Size;
	float w0 = p0.w;

	p0.xyz /= p0.w;

	float width = (2.0f / screenDimensions.x) * size;
	float height = (2.0f / screenDimensions.y) * size;

	width = width / 2.0f;
	height = height / 2.0f;

	GSOutput v[4];

	v[0].Pos = float4(p0.x - width, p0.y - height, p0.z, 1) * w0;
	v[0].Color = input[0].Color;
	v[0].Tex = float2(0, 0);

	v[1].Pos = float4(p0.x + width, p0.y - height, p0.z, 1) * w0;
	v[1].Color = input[0].Color;
	v[1].Tex = float2(1.0f, 0);

	v[2].Pos = float4(p0.x + width, p0.y + height, p0.z, 1) * w0;
	v[2].Color = input[0].Color;
	v[2].Tex = float2(1.0f, 1.0f);

	v[3].Pos = float4(p0.x - width, p0.y + height, p0.z, 1) * w0;
	v[3].Color = input[0].Color;
	v[3].Tex = float2(0.0f, 1.0f);

	output.Append(v[0]);
	output.Append(v[3]);
	output.Append(v[1]);
	output.Append(v[2]);

}