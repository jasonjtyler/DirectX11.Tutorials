#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include "LightingEffect.h"

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
};

const float blue[] = { 0.0f, 0.0f, 1.0f, 0.0f };
const float red[] = { 1.0f, 0.0f, 0.0f, 0.0f };
const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
const float gray[] = { 0.95f, 0.95f, 0.95f, 0.0f };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window 
{

private:
	ID3D11Device	*_d3dDevice;
	ID3D11DeviceContext *_d3dDeviceContext;
	IDXGISwapChain *_swapChain;
	ID3D11RenderTargetView *_renderTargetView;
	ID3D11DepthStencilView *_depthStencilView;
	ID3D11Buffer *_vertexBuffer;
	LightingEffect *_effect;
	POINT _mouseDragStart;
	HWND _handle;
	bool _mouseDragging;
	float _rotationStartX;
	float _rotationStartY;
	float _rotationX;
	float _rotationY;
	bool _displayPointLight;
	bool _displayDirectionalLight;
	
	void Destroy();
	POINT GetMousePosition();
	void Initialize();
	void InitializeVertexBuffer();
	void MouseDown();
	void MouseMove();
	void MouseUp();
	void Resize();
	void Render();

public:
	Window(HINSTANCE instance);
	HWND GetHandle();
	void SetHandle();
	LRESULT WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};