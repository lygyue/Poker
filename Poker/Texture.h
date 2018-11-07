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
#include <map>

class TextureManager;
class D3d11Texture
{
	friend class TextureManager;
public:
	std::string GetName() const
	{
		return mName;
	}
	void SetName(std::string Name)
	{
		mName = Name;
	}
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
	ID3D11ShaderResourceView** GetShaderResourceViewPtr()
	{
		return &mShaderResourceView;
	}
	DepthBuffer* GetDepthBuffer()
	{
		return mDepthBuffer;
	}

	bool BlitToTexture(unsigned char* Data, int Len);

	// this interface is use for cpu read and can't be use in shader resource.
	bool Map(BYTE*& lpData, UINT &pitch);
	void Unmap();
protected:
	D3d11Texture();
	~D3d11Texture();

	bool Initialise(std::string Name, ID3D11Device* Device, int Width, int Height, DXGI_FORMAT Format, int MipLevel, bool IsRenderTarget, bool CPUAccess);
private:
	ID3D11Texture2D*				mTexture;
	//这东西主要是要设置进渲染backbuffer，调用这个OMSetRenderTargets，设置之后，渲染的东西都会渲染到texture上
	ID3D11RenderTargetView*			mRenderTargetView;
	ID3D11ShaderResourceView*		mShaderResourceView;
	DepthBuffer*					mDepthBuffer;
	std::string						mName;
	IDXGISurface1*					mSurface;
	bool							mCpuAccess;
};

class TextureManager
{
	friend class Scene;
public:
	D3d11Texture* CreateRenderTarget(std::string Name, ID3D11Device* Device, int Width, int Height, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM);
	D3d11Texture* CreateTexture(std::string Name, ID3D11Device* Device, int Width, int Height, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM, int MipLevel = 1, bool CPUAccess = false);

	D3d11Texture* LoadTextureFromFile(std::string Name, ID3D11Device* Device, const char* FullFileName, bool IsDDS);
	D3d11Texture* LoadTextureFromMemory(std::string Name, ID3D11Device* Device, unsigned char* FileData, int FileLength, bool IsDDS);

	void DestroyTexture(D3d11Texture* Tex);
protected:
	TextureManager();
	~TextureManager();
private:
	std::map<std::string, D3d11Texture*> mTextureArray;
};