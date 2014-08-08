#include "Effect.h"

Effect::Effect(ID3D11Device *device, ID3D11DeviceContext* deviceContext)
{
	device->AddRef();
	deviceContext->AddRef();
	this->device = device;
	this->deviceContext = deviceContext;
	this->AddRef();
}

Effect::~Effect()
{
	if (this->device)
		this->device->Release();
	if (this->deviceContext)
		this->deviceContext->Release();
	if (this->inputLayout)
		this->inputLayout->Release();
	if (this->pixelShader)
		this->pixelShader->Release();
	if (this->vertexShader)
		this->vertexShader->Release();

	this->device = NULL;
	this->deviceContext = NULL;
	this->pixelShader = NULL;
	this->vertexShader = NULL;
	this->inputLayout = NULL;
}

void Effect::Initialize()
{
	this->InitializeVertexShader();
	this->InitializePixelShader();
	this->InitializeGeometryShader();
	this->InitializeBuffers();
}

void Effect::InitializeGeometryShader() {}

ULONG Effect::AddRef()
{
	InterlockedIncrement(&_refCount);
	return _refCount;
}

HRESULT Effect::QueryInterface(REFIID riid, void **ppvObject)
{
	if (!*ppvObject)
		return E_INVALIDARG;
	*ppvObject = NULL;

	if (riid == IID_IUnknown)
	{
		*ppvObject = (LPVOID)this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

ULONG Effect::Release()
{
	ULONG ulRefCount = InterlockedDecrement(&_refCount);
	if (0 == _refCount)
	{
		delete this;
	}
	return ulRefCount;
}

std::vector<byte> Effect::ReadFile(const char* filename)
{
	// open the file

	std::ifstream file(filename, std::ios::binary);

	// read the data:
	return std::vector<byte>((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
}