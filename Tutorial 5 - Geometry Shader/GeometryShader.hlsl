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
void main(lineadj VertexOut input[4], inout TriangleStream<GSOutput> output)
{
	float4 p0 = input[0].PosH;
	float4 p1 = input[1].PosH;
	float4 p2 = input[2].PosH;
	float4 p3 = input[3].PosH;

	float w0 = p1.w;
	float w1 = p2.w;

	p0.xyz /= p0.w;
	p1.xyz /= p1.w;
	p2.xyz /= p2.w;
	p3.xyz /= p3.w;

	float3 line01 = p2 - p1;
	float3 dir = normalize(line01);

	// scale to correct window aspect ratio
	float3 ratio = float3(screenDimensions.y, screenDimensions.x, 0);
	ratio = normalize(ratio);

	float width;
	if (screenDimensions.x < screenDimensions.y)
		width = (2.0f / screenDimensions.x) * lineThickness;
	else
		width = (2.0f / screenDimensions.y) * lineThickness;

	float3 normal = normalize(line01);
	float3 unit_z = float3(0, 0, 1);
	normal = (cross(normal, unit_z));
	normal.x *= ratio.x;
	normal.y *= ratio.y;
	normal *= width;

	float3 tangent = normalize(normalize(p3 - p2) + normalize(p2 - p1));
	float3 miter = normalize(cross(tangent, unit_z));
		//miter = float3(-tangent.y, tangent.x, tangent.z);
	miter.x *= ratio.x;
	miter.y *= ratio.y;

	//width = width / dot(miter, normal);
	miter *= width;

	GSOutput v[4];

	v[0].Pos = float4(p1.xyz - normal, 1.0f) * w0;
	v[0].Color = input[1].Color;
	v[0].Tex = float2(0, 0);

	v[1].Pos = float4(p1.xyz + normal, 1.0f) * w0;
	v[1].Color = input[1].Color;
	v[1].Tex = float2(0, 0);

	v[2].Pos = float4(p2.xyz + normal, 1.0f) * w1;
	v[2].Color = input[2].Color;
	v[2].Tex = float2(10.0f, 0.0f);

	v[3].Pos = float4(p2.xyz - normal, 1.0f) * w1;
	v[3].Color = input[2].Color;
	v[3].Tex = float2(10.0f, 0.0f);

	output.Append(v[2]);
	output.Append(v[1]);
	output.Append(v[0]);

	//output.Append(v[0]);
	//output.Append(v[1]);
	//output.Append(v[2]);

	output.RestartStrip();

	output.Append(v[3]);
	output.Append(v[2]);
	output.Append(v[0]);

	output.RestartStrip();
}