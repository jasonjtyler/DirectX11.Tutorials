#include "LineThicknessEffect.h"

LineThicknessEffect::LineThicknessEffect(ID3D11Device *device, ID3D11DeviceContext *deviceContext) : Effect(device, deviceContext) {}

LineThicknessEffect::~LineThicknessEffect()
{
	if (this->vertexShader)
		this->vertexShader->Release();
	if (this->pixelShader)
		this->pixelShader->Release();
	if (this->inputLayout)
		this->inputLayout->Release();
	if (this->matrixBuffer)
		this->matrixBuffer->Release();
	if (this->dimensionsBuffer)
		this->dimensionsBuffer->Release();

	this->vertexShader = NULL;
	this->pixelShader = NULL;
	this->inputLayout = NULL;
	this->matrixBuffer = NULL;
}

void LineThicknessEffect::Attach()
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->GSSetShader(geometryShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);
}

void LineThicknessEffect::InitializeBuffers()
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
	device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	D3D11_BUFFER_DESC dimensionsBufferDesc;
	ZeroMemory(&dimensionsBufferDesc, sizeof(D3D11_BUFFER_DESC));

	dimensionsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	dimensionsBufferDesc.ByteWidth = sizeof(LineParamsBuffer);
	dimensionsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dimensionsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dimensionsBufferDesc.MiscFlags = 0;
	dimensionsBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&dimensionsBufferDesc, NULL, &dimensionsBuffer);

}

void LineThicknessEffect::InitializeGeometryShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\GeometryShader.cso");
	device->CreateGeometryShader(bytes.data(), bytes.size(), NULL, &geometryShader);
}

void LineThicknessEffect::InitializePixelShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\PixelShader.cso");
	device->CreatePixelShader(bytes.data(), bytes.size(), NULL, &pixelShader);
}

void LineThicknessEffect::InitializeVertexShader()
{
	std::vector<byte> bytes = ReadFile((char*)"..\\Debug\\VertexShader.cso");
	device->CreateVertexShader(bytes.data(), bytes.size(), NULL, &vertexShader);

	const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	device->CreateInputLayout(vertexDesc, 2, bytes.data(), bytes.size(), &inputLayout);
}

D3D11_MAPPED_SUBRESOURCE LineThicknessEffect::LockDimensionsBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(dimensionsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource;
}

D3D11_MAPPED_SUBRESOURCE LineThicknessEffect::LockMatrixBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource;
}

void LineThicknessEffect::UnlockDimensionsBuffer()
{
	deviceContext->Unmap(dimensionsBuffer, 0);
	deviceContext->GSSetConstantBuffers(0, 1, &dimensionsBuffer);
}

void LineThicknessEffect::UnlockMatrixBuffer()
{
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
}