#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <vector>
#include <fstream>
#include "PointsEffect.h"

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	float Size;
};


const float blue[] = { 0.0f, 0.0f, 1.0f, 0.0f };
const float red[] = { 1.0f, 0.0f, 0.0f, 0.0f };
const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window 
{

private:
	ID3D11Device* _d3dDevice;
	ID3D11DeviceContext* _d3dDeviceContext;
	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Buffer* _vertexBuffer;
	PointsEffect* _effect;
	ID3D11BlendState* _transparencyBlend;
	ID3D11Resource* _texture;
	ID3D11ShaderResourceView* _textureView;
	HWND _handle;
	float _pointSize;
	float _width;
	float _height;
	
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