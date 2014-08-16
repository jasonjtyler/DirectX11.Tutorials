struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
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
	float lineThickness;
	float pad;
};

[maxvertexcount(6)]
void main(line VertexOut input[2], inout TriangleStream<GSOutput> output)
{
	float4 p0 = input[0].PosH;
	float4 p1 = input[1].PosH;

	float w0 = p0.w;
	float w1 = p1.w;

	p0.xyz /= p0.w;
	p1.xyz /= p1.w;

	float3 line01 = p1 - p0;
	float3 dir = normalize(line01);

	// scale to correct window aspect ratio
	float3 ratio = float3(screenDimensions.x, screenDimensions.y, 0);
	ratio = normalize(ratio);

	float3 unit_z = normalize(float3(0, 0, -1));

	float3 normal = normalize(cross(unit_z, dir) * ratio);

	float width = 0.01;
	if (screenDimensions.x > screenDimensions.y)
		width = (2.0f / screenDimensions.x) * lineThickness;
	else
		width = (2.0f / screenDimensions.y) * lineThickness;

	GSOutput v[4];

	float3 dir_offset = dir * ratio * width;
	float3 normal_scaled = normal * ratio * width;

	float3 p0_ex = p0 - dir_offset;
	float3 p1_ex = p1 + dir_offset;

	v[0].Pos = float4(p0_ex - normal_scaled, 1) * w0;
	v[0].Color = input[0].Color;
	v[0].Tex = float2(0, 0);

	v[1].Pos = float4(p0_ex + normal_scaled, 1) * w0;
	v[1].Color = input[0].Color;
	v[1].Tex = float2(0, 0);

	v[2].Pos = float4(p1_ex + normal_scaled, 1) * w1;	
	v[2].Color = input[1].Color;
	v[2].Tex = float2(10.0f, 0.0f);

	v[3].Pos = float4(p1_ex - normal_scaled, 1) * w1;
	v[3].Color = input[1].Color;
	v[3].Tex = float2(10.0f, 0.0f);

	output.Append(v[2]);
	output.Append(v[1]);
	output.Append(v[0]);

	output.RestartStrip();

	output.Append(v[3]);
	output.Append(v[2]);
	output.Append(v[0]);

	output.RestartStrip();
}