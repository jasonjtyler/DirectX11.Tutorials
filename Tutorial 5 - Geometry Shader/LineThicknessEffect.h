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

class LineThicknessEffect : public Effect
{

public:
	LineThicknessEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~LineThicknessEffect();

	void Attach();
	D3D11_MAPPED_SUBRESOURCE LockDimensionsBuffer();
	D3D11_MAPPED_SUBRESOURCE LockMatrixBuffer();
	void UnlockDimensionsBuffer();
	void UnlockMatrixBuffer();

protected:
	void InitializeBuffers();
	void InitializeGeometryShader();
	void InitializePixelShader();
	void InitializeVertexShader();

private:
	ID3D11Buffer *matrixBuffer;
	ID3D11Buffer *dimensionsBuffer;

};