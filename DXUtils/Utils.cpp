#pragma once
#include "Utils.h"


DirectX::XMMATRIX Utils::InverseTranspose(const DirectX::XMMATRIX &matrix)
{
	DirectX::XMMATRIX a = DirectX::XMMATRIX(matrix);
	a.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(a);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, a));
}

DirectX::XMVECTOR Utils::ComputeNormal(const DirectX::XMVECTOR &p0, const DirectX::XMVECTOR &p1, const DirectX::XMVECTOR &p2)
{
	DirectX::XMVECTOR u = DirectX::XMVectorSubtract(p1, p0);
	DirectX::XMVECTOR v = DirectX::XMVectorSubtract(p2, p0);

	u = DirectX::XMVector3Cross(u, v);
	u = DirectX::XMVector3Normalize(u);

	return u;
}

HRESULT Utils::CreateDepthStencilView(ID3D11Device *device, D3D11_TEXTURE2D_DESC *description, ID3D11DepthStencilView **depthStencilView)
{
	HRESULT result;
	ID3D11Texture2D *depthStencilBuffer = NULL;

	result = device->CreateTexture2D(description, NULL, &depthStencilBuffer);
	result = device->CreateDepthStencilView(depthStencilBuffer, NULL, depthStencilView);

	depthStencilBuffer->Release();
	return result;
}

D3D11_TEXTURE2D_DESC Utils::CreateDepthStencilDescription(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	return depthStencilDesc;
}

HRESULT Utils::CreateSwapChain(ID3D11Device *device, DXGI_SWAP_CHAIN_DESC *description, IDXGISwapChain **swapChain)
{
	HRESULT result;
	
	IDXGIDevice* dxgiDevice = NULL;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = NULL;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = NULL;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	result = dxgiFactory->CreateSwapChain(device, description, swapChain);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return result;
}

DXGI_SWAP_CHAIN_DESC Utils::CreateSwapChainDescription(UINT width, UINT height, HWND outputWindow)
{
	DXGI_SWAP_CHAIN_DESC swapChain;

	swapChain.BufferDesc.Width = width;
	swapChain.BufferDesc.Height = height;
	swapChain.BufferDesc.RefreshRate.Numerator = 60;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;
	swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChain.SampleDesc.Count = 1;
	swapChain.SampleDesc.Quality = 0;
	swapChain.Flags = 0;

	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChain.BufferCount = 1;
	swapChain.OutputWindow = outputWindow;
	swapChain.Windowed = true;
	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChain.Flags = 0;

	return swapChain;
}

void Utils::SetViewport(ID3D11DeviceContext *deviceContext, float width, float height)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &vp);
}