struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW: POSITION;
	float4 Color : COLOR;
	float3 NormalW : NORMAL;
};

cbuffer cbPerFrame
{
	DirectionalLight gLight;
	float3 eyePosition;
	float pad;
};

void computeDirectionalLight(float4 surfaceColor, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = gLight.Ambient * surfaceColor;
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	normal = normalize(normal);

	float3 lightVector = -gLight.Direction;

	float diffuseFactor = dot(lightVector, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 reflection = reflect(-lightVector, normal);
	    float specularFactor = pow(max(dot(reflection, toEye), 0.0f), 2.0f);

		diffuse = diffuseFactor * surfaceColor * gLight.Diffuse;
		specular = specularFactor * gLight.Specular;
	}

}

float4 main(VertexOut pin) : SV_TARGET
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 eye = normalize(eyePosition - pin.PosW);

	computeDirectionalLight(pin.Color, pin.NormalW, eye, ambient, diffuse, specular);

	float4 litColor = ambient + diffuse + specular;
	litColor.a = 1.0f;

	return litColor;
}
