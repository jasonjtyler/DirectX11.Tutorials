struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Position;
	float Range;
	float3 Attenuation;
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
	DirectionalLight gDirectionalLight;
	PointLight gPointLight;
	float3 eyePosition;
	float pad;
};

void computeDirectionalLight(float4 surfaceColor, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = gDirectionalLight.Ambient * surfaceColor;

	float3 lightVector = -gDirectionalLight.Direction;

	float diffuseFactor = dot(lightVector, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 reflection = reflect(-lightVector, normal);
	    float specularFactor = pow(max(dot(reflection, toEye), 0.0f), 2.0f);

		diffuse = diffuseFactor * surfaceColor * gDirectionalLight.Diffuse;
		specular = specularFactor * gDirectionalLight.Specular;
	}

}

void computePointLight(float4 surfaceColor, float3 normal, float3 position, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVector = gPointLight.Position - position;
	float distance = length(lightVector);

	if (distance > gPointLight.Range)
		return;

	lightVector /= distance;

	ambient = gPointLight.Ambient * surfaceColor;

	float diffuseFactor = dot(lightVector, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 reflection = reflect(-lightVector, normal);
		float specularFactor = pow(max(dot(reflection, toEye), 0.0f), 10.0f);

		diffuse = diffuseFactor * surfaceColor * gPointLight.Diffuse;
		specular = specularFactor * gPointLight.Specular;
	}
	
	float att = 1.0f / dot(gPointLight.Attenuation, float3(1.0f, distance, distance*distance));

	diffuse *= att;
	specular *= att;
}

float4 main(VertexOut pin) : SV_TARGET
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 totalAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 totalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 totalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 toEye = normalize(eyePosition - pin.PosW);

	float3 normalizedNormal = normalize(pin.NormalW);

	computeDirectionalLight(pin.Color, normalizedNormal, toEye, ambient, diffuse, specular);
	totalAmbient += ambient;
	totalDiffuse += diffuse;
	totalSpecular += specular;

	computePointLight(pin.Color, normalizedNormal, pin.PosW, toEye, ambient, diffuse, specular);
	totalAmbient += ambient;
	totalDiffuse += diffuse;
	totalSpecular += specular;

	float4 litColor = totalAmbient + totalDiffuse + totalSpecular;
	litColor.a = 1.0f;

	return litColor;
}
