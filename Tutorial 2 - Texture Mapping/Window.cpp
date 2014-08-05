// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
#pragma once
#include "Window.h"
#include "Utils.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("DirectX Tutorial #2 - Texture Mapping");

Window::Window(HINSTANCE instance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 8;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND handle = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 400,
		NULL,
		NULL,
		instance,
		NULL
		);

	if (!handle)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);
	}

	this->_handle = handle;
	SetWindowLongPtr(handle, GWL_USERDATA, reinterpret_cast<long>(this));

	_rotationStartX = 0.0f;
	_rotationStartY = 0.0f;

	_rotationX = 0.0f;
	_rotationY = 0.0f;


}

void Window::Destroy()
{
	if (_effect)
		_effect->Release();
	if (_vertexBuffer)
		_vertexBuffer->Release();
	if (_depthStencilView)
		_depthStencilView->Release();
	if (_renderTargetView)
		_renderTargetView->Release();
	if (_texture)
		_texture->Release();
	if (_textureView)
		_textureView->Release();
	if (_swapChain)
		_swapChain->Release();
	if (_d3dDeviceContext)
		_d3dDeviceContext->Release();
	if (_d3dDevice)
		_d3dDevice->Release();
}

HWND Window::GetHandle()
{
	return this->_handle;
}

POINT Window::GetMousePosition()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_handle, &p);
	return p;
}

void Window::Initialize()
{
	D3D_FEATURE_LEVEL level;

	HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &_d3dDevice, &level, &_d3dDeviceContext);

	Resize();

	InitializeVertexBuffer();
	InitializeTexture();

	_effect = new TextureEffect(_d3dDevice, _d3dDeviceContext);
	_effect->Initialize();
}

void Window::InitializeVertexBuffer()
{
	Vertex vertices[] = {

		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },

		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },

		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(Vertex) * 36;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;

	_d3dDevice->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
}

void Window::InitializeTexture()
{
	HRESULT result = DirectX::CreateWICTextureFromFile(_d3dDevice, L"test.png", &_texture, &_textureView, 0);
}

void Window::MouseDown()
{
	if (!_mouseDragging)
	{
		_mouseDragging = true;
		_mouseDragStart = GetMousePosition();
	}
}

void Window::MouseMove()
{
	if (_mouseDragging)
	{
		RECT clientRect;
		GetClientRect(_handle, &clientRect);

		POINT p = GetMousePosition();

		_rotationY = (float)(_mouseDragStart.x - p.x) / (float)clientRect.right;
		_rotationY *= 3.14f;
		_rotationY += _rotationStartY;

		_rotationX = (float)(_mouseDragStart.y - p.y) / (float)clientRect.bottom;
		_rotationX *= 3.14f;
		_rotationX += _rotationStartX;

	}
}

void Window::MouseUp()
{
	_mouseDragging = false;
	_rotationStartX = _rotationX;
	_rotationStartY = _rotationY;
}

void Window::Render()
{
	RECT clientRect;
	GetClientRect(_handle, &clientRect);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0.0, 0.0, -5.0, 0);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1.0f, 0, 0);
	DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(pos, target, up);

	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(0.5f, ((float)clientRect.right / (float)clientRect.bottom), 0.01f, 1000.0f);

	DirectX::XMMATRIX world = DirectX::XMMatrixRotationY(_rotationY) * DirectX::XMMatrixRotationX(_rotationX);

	DirectX::XMMATRIX wvp = XMMatrixTranspose((world * v) * p);

	D3D11_MAPPED_SUBRESOURCE resource = _effect->LockMatrixBuffer();
	DirectX::XMStoreFloat4x4(&(((MatrixBuffer*)resource.pData)->WorldViewProj), wvp);
	_effect->UnlockMatrixBuffer();

	_effect->SetTexture(_textureView);

	_d3dDeviceContext->ClearRenderTargetView(_renderTargetView, gray);
	_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	_d3dDeviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	_effect->Attach();

	_d3dDeviceContext->Draw(36, 0);

	_swapChain->Present(0, 0);
}

void Window::Resize()
{
	HRESULT result;

	RECT clientRect;
	GetClientRect(_handle, &clientRect);

	if (_swapChain)
		_swapChain->Release();
	if (_depthStencilView)
		_depthStencilView->Release();
	if (_renderTargetView)
		_renderTargetView->Release();

	//Swap chain

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc = Utils::CreateSwapChainDescription(clientRect.right, clientRect.bottom, _handle);
	result = Utils::CreateSwapChain(_d3dDevice, &swapChainDesc, &_swapChain);

	//Render target view

	ID3D11Texture2D* backBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	_d3dDevice->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
	backBuffer->Release();

	//Depth stencil

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc = Utils::CreateDepthStencilDescription(clientRect.right, clientRect.bottom);
	result = Utils::CreateDepthStencilView(_d3dDevice, &depthStencilDesc, &_depthStencilView);

	//Set depth and stencil views

	_d3dDeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	//Set viewport

	Utils::SetViewport(_d3dDeviceContext, (float)clientRect.right, (float)clientRect.bottom);
}

LRESULT Window::WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
		Initialize();
		break;

	case WM_SIZE:
		Resize();
		break;

	case WM_PAINT:
		Render();
		break;

	case WM_LBUTTONDOWN:
		MouseDown();
		break;

	case WM_MOUSEMOVE:
		MouseMove();
		break;

	case WM_LBUTTONUP:
		MouseUp();
		break;

	case WM_DESTROY:
		Destroy();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window *window;

	window = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWL_USERDATA));
	if (window)
		return window->WindowMessage(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);

}