/*!
 * \file DataBuffer.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Common.h"

struct DepthBuffer
{
	ID3D11DepthStencilView * TexDsv;

	DepthBuffer(ID3D11Device * Device, int sizeW, int sizeH, int sampleCount = 1);

	~DepthBuffer();
};

struct DataBuffer
{
	ID3D11Buffer * D3DBuffer;
	size_t         Size;

	DataBuffer(ID3D11Device * Device, D3D11_BIND_FLAG use, const void* buffer, size_t size);

	~DataBuffer();
};
