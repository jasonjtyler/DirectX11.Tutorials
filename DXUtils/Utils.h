#include <stdlib.h>
#include <d3d11.h>

class Utils : IUnknown
{

public:
	static HRESULT CreateDepthStencilView(ID3D11Device *device, D3D11_TEXTURE2D_DESC *description, ID3D11DepthStencilView **depthStencilView);
	static D3D11_TEXTURE2D_DESC CreateDepthStencilDescription(UINT width, UINT height);
	static HRESULT CreateSwapChain(ID3D11Device *device, DXGI_SWAP_CHAIN_DESC *description, IDXGISwapChain **swapChain);
	static DXGI_SWAP_CHAIN_DESC CreateSwapChainDescription(UINT width, UINT height, HWND outputWindow);
	static void SetViewport(ID3D11DeviceContext *deviceContext, float width, float height);
};