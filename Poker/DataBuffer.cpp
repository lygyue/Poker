/*!
 * \file DataBuffer.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "DataBuffer.h"

DepthBuffer::DepthBuffer(ID3D11Device * Device, int sizeW, int sizeH, int sampleCount /* = 1 */)
{
	DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Width = sizeW;
	dsDesc.Height = sizeH;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = format;
	dsDesc.SampleDesc.Count = sampleCount;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ID3D11Texture2D * Tex;
	Device->CreateTexture2D(&dsDesc, NULL, &Tex);
	Device->CreateDepthStencilView(Tex, NULL, &TexDsv);
	Tex->Release();
}

DepthBuffer::~DepthBuffer()
{
	SAFE_RELEASE(TexDsv);
}

DataBuffer::DataBuffer(ID3D11Device * Device, D3D11_BIND_FLAG use, const void* buffer, size_t size) : Size(size)
{
	D3D11_BUFFER_DESC desc;   memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = use;
	desc.ByteWidth = (unsigned)size;
	D3D11_SUBRESOURCE_DATA sr;
	sr.pSysMem = buffer;
	sr.SysMemPitch = sr.SysMemSlicePitch = 0;
	HRESULT hr = Device->CreateBuffer(&desc, buffer ? &sr : NULL, &D3DBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringA("DataBuffer::CreateBuffer Failed");
	}
}

DataBuffer::~DataBuffer()
{
	SAFE_RELEASE(D3DBuffer);
}
