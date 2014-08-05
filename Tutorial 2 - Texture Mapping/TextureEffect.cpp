#include "TextureEffect.h"

TextureEffect::TextureEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext) : Effect(device, deviceContext) {}

TextureEffect::~TextureEffect()
{
	if (_matrixBuffer)
		_matrixBuffer->Release();
}

void TextureEffect::Attach()
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);
}

void TextureEffect::InitializeBuffers()
{
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
}

void TextureEffect::InitializePixelShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\PixelShader.cso");
	device->CreatePixelShader(bytes.data(), bytes.size(), NULL, &pixelShader);
}

void TextureEffect::InitializeVertexShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\VertexShader.cso");
	device->CreateVertexShader(bytes.data(), bytes.size(), NULL, &vertexShader);

	const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(vertexDesc, 2, bytes.data(), bytes.size(), &inputLayout);
}

D3D11_MAPPED_SUBRESOURCE TextureEffect::LockMatrixBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource;
}

void TextureEffect::SetTexture(ID3D11ShaderResourceView *texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
}

void TextureEffect::UnlockMatrixBuffer()
{
	deviceContext->Unmap(_matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);
}