#include <stdlib.h>
#include <vector>
#include <fstream>
#include <d3d11.h>

class Effect : IUnknown
{

public:
	Effect(ID3D11Device *device, ID3D11DeviceContext* deviceContext);
	virtual ~Effect();

	void Initialize();
	ULONG STDMETHODCALLTYPE AddRef(void);
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	ULONG STDMETHODCALLTYPE Release(void);

	virtual void Attach() = 0;

protected:
	ID3D11Device *device;
	ID3D11DeviceContext* deviceContext;
	ID3D11InputLayout *inputLayout;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11GeometryShader *geometryShader;

	std::vector<byte> ReadFile(const char* filename);

	virtual void InitializeGeometryShader();

	virtual void InitializeBuffers() = 0;
	virtual void InitializePixelShader() = 0;
	virtual void InitializeVertexShader() = 0;

private:
	LONG _refCount;

};