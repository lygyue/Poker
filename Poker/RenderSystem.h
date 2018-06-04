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
	DataBuffer* GetUniformBuffer() const;
protected:
	RenderSystemD3D11();
	~RenderSystemD3D11();

	bool InitialiseD3d11(int Width, int Height, HWND Hwnd);

	void SetAndClearRenderTarget()
	{
		float black[] = { mBackColor.x, mBackColor.y, mBackColor.z, mBackColor.w }; // Important that alpha=0, if want pixels to be transparent, for manual layers
		mDX11DeviceContext->OMSetRenderTargets(1, &mBackRenderTargetView, mBackDepth->TexDsv);
		mDX11DeviceContext->ClearRenderTargetView(mBackRenderTargetView, black);
		if (mBackDepth)
			mDX11DeviceContext->ClearDepthStencilView(mBackDepth->TexDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
	void SetViewport(float vpX, float vpY, float vpW, float vpH)
	{
		D3D11_VIEWPORT D3Dvp;
		D3Dvp.Width = vpW;    D3Dvp.Height = vpH;
		D3Dvp.MinDepth = 0;   D3Dvp.MaxDepth = 1;
		D3Dvp.TopLeftX = vpX; D3Dvp.TopLeftY = vpY;
		mDX11DeviceContext->RSSetViewports(1, &D3Dvp);
	}

	void Present()
	{
		mSwapChain1->Present(0, 0);
	}
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
	XMFLOAT4						mBackColor;
	D3D11_PRIMITIVE_TOPOLOGY		mPrimitiveType;
};
