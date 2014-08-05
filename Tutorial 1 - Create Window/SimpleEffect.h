#include <Effect.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <DirectXMath.h>

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};

class SimpleEffect : public Effect
{

public:
	SimpleEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~SimpleEffect();

	void Attach();
	D3D11_MAPPED_SUBRESOURCE LockMatrixBuffer();
	void UnlockMatrixBuffer();

protected:
	void InitializeBuffers();
	void InitializePixelShader();
	void InitializeVertexShader();

private:
	ID3D11Buffer *matrixBuffer;

};