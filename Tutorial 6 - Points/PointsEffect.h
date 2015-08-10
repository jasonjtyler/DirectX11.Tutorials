#include <Effect.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <DirectXMath.h>

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};

struct LineParamsBuffer
{
	DirectX::XMFLOAT2 ScreenDimensions;
	float LineThickness;
	float pad;
};

class PointsEffect : public Effect
{

public:
	PointsEffect(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~PointsEffect();

	void Attach();
	D3D11_MAPPED_SUBRESOURCE LockDimensionsBuffer();
	D3D11_MAPPED_SUBRESOURCE LockMatrixBuffer();
	void SetTexture(ID3D11ShaderResourceView *textureView);
	void UnlockDimensionsBuffer();
	void UnlockMatrixBuffer();

protected:
	void InitializeBuffers();
	void InitializeGeometryShader();
	void InitializePixelShader();
	void InitializeVertexShader();

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* dimensionsBuffer;

};