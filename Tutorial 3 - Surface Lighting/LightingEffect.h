#include <Effect.h>
#include <DirectXMath.h>

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Direction;
	float pad;
};

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 WorldViewProj;
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 WorldInverse;
};

struct LightsBuffer
{
	DirectionalLight DirectionLight1;
	DirectX::XMFLOAT3 EyePosition;
	float pad;
};

class LightingEffect : public Effect
{
public:
	LightingEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~LightingEffect();

	void Attach();
	D3D11_MAPPED_SUBRESOURCE LockMatrixBuffer();
	D3D11_MAPPED_SUBRESOURCE LockDirectionalLightBuffer();
	void UnlockMatrixBuffer();
	void UnlockDirectionalLightBuffer();

private:
	ID3D11Buffer *_matrixBuffer;
	ID3D11Buffer *_directionalLightBuffer;

	void InitializeBuffers();
	void InitializePixelShader();
	void InitializeVertexShader();

};