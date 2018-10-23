/*!
 * \file Ô´RenderSystem.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "RenderSystem.h"

RenderSystemD3D11::RenderSystemD3D11()
{
	mD3dDevice11 = nullptr;
	mDX11DeviceContext = nullptr;
	mBackTexture = nullptr;
	mBackDepth = nullptr;
	mSwapChain1 = nullptr;
	mBackRenderTargetView = nullptr;
	mUniformBufferGen = nullptr;
	mWidth = 0;
	mHeight = 0;
	//mPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	mPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	mBackColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

RenderSystemD3D11::~RenderSystemD3D11()
{
	SAFE_DELETE(mUniformBufferGen);
	SAFE_DELETE(mBackDepth);
	SAFE_RELEASE(mSwapChain1);
	SAFE_RELEASE(mBackTexture);
	SAFE_RELEASE(mBackRenderTargetView);
	SAFE_RELEASE(mDX11DeviceContext);
	SAFE_RELEASE(mD3dDevice11);
}

bool RenderSystemD3D11::Initialise(int Width, int Height, HWND Hwnd)
{
	mWidth = Width;
	mHeight = Height;
	// Create Device and Context
	IDXGIAdapter* Adapter = nullptr;
	D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	HRESULT hr = D3D11CreateDevice(Adapter
		, D3D_DRIVER_TYPE_HARDWARE
		, 0
		, createFlags
		, FeatureLevels
		, NumFeatureLevels
		, D3D11_SDK_VERSION
		, &mD3dDevice11
		, NULL
		, &mDX11DeviceContext
	);
	if (FAILED(hr))
	{
		OutputDebugStringA("D3D11CreateDevice Failed!");
		return false;
	}

	// Create SwapChain
	IDXGIFactory1 * DXGIFactory = nullptr;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&DXGIFactory));
	if (FAILED(hr))
	{
		OutputDebugStringA("CreateDXGIFactory1 Failed!");
		return false;
	}
	DXGI_SWAP_CHAIN_DESC scDesc;
	memset(&scDesc, 0, sizeof(scDesc));
	scDesc.BufferCount = 2;
	scDesc.BufferDesc.Width = Width;//Width;
	scDesc.BufferDesc.Height = Height;//Height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = Hwnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.Windowed = true;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	IDXGISwapChain* SwapChain = NULL;
	hr = DXGIFactory->CreateSwapChain(mD3dDevice11, &scDesc, &SwapChain);
	if (FAILED(hr))
	{
		OutputDebugStringA("CreateSwapChain Failed!");
		return false;
	}
	mSwapChain1 = (IDXGISwapChain1*)SwapChain;
	SAFE_RELEASE(DXGIFactory);
	mBackDepth = new DepthBuffer(mD3dDevice11, Width, Height);
	mSwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&mBackTexture);
	hr = mD3dDevice11->CreateRenderTargetView(mBackTexture, NULL, &mBackRenderTargetView);
	mDX11DeviceContext->OMSetRenderTargets(1, &mBackRenderTargetView, mBackDepth->TexDsv);

	mUniformBufferGen = new DataBuffer(mD3dDevice11, D3D11_BIND_CONSTANT_BUFFER, NULL, UNIFORM_DATA_SIZE);
	mDX11DeviceContext->VSSetConstantBuffers(0, 1, &mUniformBufferGen->D3DBuffer);
	mDX11DeviceContext->PSSetConstantBuffers(0, 1, &mUniformBufferGen->D3DBuffer);
	return true;
}

ID3D11Device* RenderSystemD3D11::GetD3d11Device() const
{
	return mD3dDevice11;
}

ID3D11DeviceContext* RenderSystemD3D11::GetD3d11Context() const
{
	return mDX11DeviceContext;
}

DataBuffer* RenderSystemD3D11::GetUniformBuffer() const
{
	return mUniformBufferGen;
}