#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <vector>
#include <fstream>
#include "SimpleEffect.h"

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};


const float blue[] = { 0.0f, 0.0f, 1.0f, 0.0f };
const float red[] = { 1.0f, 0.0f, 0.0f, 0.0f };
const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };

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
	ID3D11InputLayout *_layout;
	ID3D11VertexShader *_vertexShader;
	ID3D11PixelShader *_pixelShader;
	SimpleEffect *_effect;
	HWND _handle;
	
	void Destroy();
	void Initialize();
	void InitializeVertexBuffer();
	void InitializeShaders();
	void Resize();
	void Render();
	std::vector<byte> ReadFile(const char* filename);

public:
	Window(HINSTANCE instance);
	HWND GetHandle();
	void SetHandle();
	LRESULT WindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};