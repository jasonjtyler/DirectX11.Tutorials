#include <Effect.h>
#include <DirectXMath.h>

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};

class TextureEffect : public Effect
{
public:
	TextureEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~TextureEffect();

	void Attach();
	D3D11_MAPPED_SUBRESOURCE LockMatrixBuffer();
	void SetTexture(ID3D11ShaderResourceView *texture);
	void UnlockMatrixBuffer();

private:
	ID3D11Buffer *_matrixBuffer;

	void InitializeBuffers();
	void InitializePixelShader();
	void InitializeVertexShader();

};