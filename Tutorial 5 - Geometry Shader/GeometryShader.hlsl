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

static float3 unit_z = float3(0, 0, 1.0f);

[maxvertexcount(6)]
void main(lineadj VertexOut input[4], inout TriangleStream<GSOutput> output)
{
	float4 p0 = input[0].PosH;
	float4 p1 = input[1].PosH;
	float4 p2 = input[2].PosH;
	float4 p3 = input[3].PosH;

	float w0 = p1.w;
	float w1 = p2.w;

	//Perspective divide to apply perspective to homogeneous coordinates.
	p0.xyz /= p0.w;
	p1.xyz /= p1.w;
	p2.xyz /= p2.w;
	p3.xyz /= p3.w;

	//Transform to screen coordinates.
	float4 screenDims = float4(screenDimensions.x, screenDimensions.y, 1.0f, 1.0f);
	p0.xy *= screenDims.xy;
	p1.xy *= screenDims.xy;
	p2.xy *= screenDims.xy;
	p3.xy *= screenDims.xy;

	//Get the line direction and normal.
	float2 line1 = normalize(p2.xy - p1.xy);
	float2 normal1 = float2(-line1.y, line1.x);

	float2 miter1;
	float2 miter2;
	float2 tangent;

	//if p0 == p1, this is an endpoint. Use the normal to cap the line.
	if (p0.x == p1.x && p0.y == p1.y)
	{
		miter1 = normal1;
		miter1 *= lineThickness;
	}
	else
	{
		//Use the adjacent line to find a tangent.
		float2 line0 = normalize(p1 - p0);
		tangent = normalize(line0 + line1);
		
		//The normal of the tangent can be used as a line miter.
		miter1 = float2(-tangent.y, tangent.x);
		
		//Scale the miter to the thickness.
		miter1 *= lineThickness / dot(miter1, normal1);
	}

	//If p2 == p3, this is an endpoint. Use the normal to cap the line.
	if (p2.x == p3.x && p2.y == p3.y)
	{
		miter2 = normal1;
		miter2 *= lineThickness;
	}
	else
	{
		//Use the adjacent line to find a tangent.
		float2 line2 = normalize(p3 - p2);
		tangent = normalize(line2 + line1);

		//The normal of the tangent can be used as a line miter.
		miter2 = float2(-tangent.y, tangent.x);

		//Scale the miter to the thickness.
		miter2 *= lineThickness / dot(miter2, normal1);
	}

	GSOutput v[4];

	//Create 4 vertices that will be used to create a quad.
	v[0].Pos = float4(p1.xy - miter1, p1.z, 1.0f);
	v[0].Color = input[1].Color;
	v[0].Tex = float2(0, 0);

	v[1].Pos = float4(p1.xy + miter1, p1.z, 1.0f);
	v[1].Color = input[1].Color;
	v[1].Tex = float2(0, 0);

	v[2].Pos = float4(p2.xy + miter2, p2.z, 1.0f);
	v[2].Color = input[2].Color;
	v[2].Tex = float2(10.0f, 0.0f);

	v[3].Pos = float4(p2.xy - miter2, p2.z, 1.0f);
	v[3].Color = input[2].Color;
	v[3].Tex = float2(10.0f, 0.0f);


	//Transform back to homogeneous coordinates.
	v[0].Pos /= screenDims;
	v[1].Pos /= screenDims;
	v[2].Pos /= screenDims;
	v[3].Pos /= screenDims;

	//Add the w component back.
	v[0].Pos *= w0;
	v[1].Pos *= w0;
	v[2].Pos *= w1;
	v[3].Pos *= w1;

	//Create two triangles to form the quad.

	output.Append(v[0]);
	output.Append(v[1]);
	output.Append(v[2]);

	output.RestartStrip();

	output.Append(v[0]);
	output.Append(v[2]);
	output.Append(v[3]);

	output.RestartStrip();
}