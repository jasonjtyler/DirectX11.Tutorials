#include "LightingEffect.h"

LightingEffect::LightingEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext) : Effect(device, deviceContext) {}

LightingEffect::~LightingEffect()
{
	if (_matrixBuffer)
		_matrixBuffer->Release();
}

void LightingEffect::Attach()
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);
}

void LightingEffect::InitializeBuffers()
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

	// Create the constant buffer pointer so it can be accessed from the vertex shader constant buffer from within this class.
	device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);

	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(LightsBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&matrixBufferDesc, NULL, &_directionalLightBuffer);

}

void LightingEffect::InitializePixelShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\PixelShader.cso");
	device->CreatePixelShader(bytes.data(), bytes.size(), NULL, &pixelShader);
}

void LightingEffect::InitializeVertexShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\VertexShader.cso");
	device->CreateVertexShader(bytes.data(), bytes.size(), NULL, &vertexShader);

	const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(vertexDesc, 3, bytes.data(), bytes.size(), &inputLayout);
}

D3D11_MAPPED_SUBRESOURCE LightingEffect::LockMatrixBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource;
}

D3D11_MAPPED_SUBRESOURCE LightingEffect::LockDirectionalLightBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(_directionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource;
}

void LightingEffect::UnlockMatrixBuffer()
{
	deviceContext->Unmap(_matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);
}

void LightingEffect::UnlockDirectionalLightBuffer()
{
	deviceContext->Unmap(_directionalLightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &_directionalLightBuffer);
}