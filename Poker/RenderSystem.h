/*!
 * \file RenderSystem.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once

#include "Common.h"
#include "DataBuffer.h"

class RenderSystemD3D11
{
	friend class Scene;
public:
	ID3D11Device* GetD3d11Device() const;
	ID3D11DeviceContext* GetD3d11Context() const;
protected:
	RenderSystemD3D11();
	~RenderSystemD3D11();

	bool InitialiseD3d11(int Width, int Height, HWND Hwnd);

private:
	static const int UNIFORM_DATA_SIZE = 2048;
	int mWidth;
	int mHeight;
	// d3d11 relativity
	ID3D11Device*					mD3dDevice11;
	ID3D11DeviceContext*			mDX11DeviceContext;
	IDXGISwapChain1*				mSwapChain1;
	ID3D11Texture2D*				mBackTexture;
	ID3D11RenderTargetView*			mBackRenderTargetView;
	DepthBuffer*					mBackDepth;
	DataBuffer*						mUniformBufferGen;
	unsigned char					mUniformData[UNIFORM_DATA_SIZE];
	XMFLOAT4						mMasterCol;
	XMFLOAT4						mBackColor;
	D3D11_PRIMITIVE_TOPOLOGY		mPrimitiveType;
};
