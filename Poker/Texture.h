/*!
 * \file Texture.h
 *
 * \author LYG
 * \date 四月 2018
 *
 * 
 */
#pragma once
#include "Common.h"
#include "DataBuffer.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

class D3d11Texture
{
public:
	static D3d11Texture* CreateRenderTarget(ID3D11Device* Device, int Width, int Height, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM);
	static D3d11Texture* CreateTexture(ID3D11Device* Device, int Width, int Height, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM, int MipLevel = 1);

	static D3d11Texture* LoadTextureFromFile(ID3D11Device* Device, const char* FullFileName, bool IsDDS);
	static D3d11Texture* LoadTextureFromMemory(ID3D11Device* Device, unsigned char* FileData, int FileLength, bool IsDDS);


	void Release();

	ID3D11Texture2D* GetTexture()
	{
		return mTexture;
	}
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return mRenderTargetView;
	}
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return mShaderResourceView;
	}
	DepthBuffer* GetDepthBuffer()
	{
		return mDepthBuffer;
	}
protected:
	D3d11Texture();
	~D3d11Texture();

	bool Initialise(ID3D11Device* Device, int Width, int Height, DXGI_FORMAT Format, int MipLevel, bool IsRenderTarget);
private:
	ID3D11Texture2D*				mTexture;
	//这东西主要是要设置进渲染backbuffer，调用这个OMSetRenderTargets，设置之后，渲染的东西都会渲染到texture上
	ID3D11RenderTargetView*			mRenderTargetView;
	ID3D11ShaderResourceView*		mShaderResourceView;
	DepthBuffer*					mDepthBuffer;
};