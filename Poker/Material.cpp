/*!
 * \file Material.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */

#include "Material.h"
#include "Scene.h"

std::string StandardMaterialName[CutomShader] = { "Simple_Black", "Simple_White", "Simple_Red", "Simple_Green", "Simple_Blue", "Simple_Texture_Sample" };
Material::Material()
{
	mTex = nullptr;
	mShader = nullptr;

	HRESULT result = S_OK;
	ID3D11Device* Device = Scene::GetCurrentScene()->GetRenderSystem()->GetD3d11Device();
	// Create sampler state
	D3D11_SAMPLER_DESC ss; memset(&ss, 0, sizeof(ss));
	ss.AddressU = ss.AddressV = ss.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	ss.Filter = D3D11_FILTER_ANISOTROPIC;
	ss.MaxAnisotropy = 8;
	ss.MaxLOD = 15;
	result = Device->CreateSamplerState(&ss, &mSamplerState);

	// Create rasterizer
	D3D11_RASTERIZER_DESC rs; memset(&rs, 0, sizeof(rs));
	rs.AntialiasedLineEnable = rs.DepthClipEnable = true;
	rs.CullMode = D3D11_CULL_BACK;
	rs.FillMode = D3D11_FILL_SOLID;
	result = Device->CreateRasterizerState(&rs, &mRasterizer);

	// Create depth state
	D3D11_DEPTH_STENCIL_DESC dss;
	memset(&dss, 0, sizeof(dss));
	dss.DepthEnable = true;
	dss.DepthFunc = D3D11_COMPARISON_LESS;
	dss.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	result = Device->CreateDepthStencilState(&dss, &mDepthState);

	//Create blend state - trans or otherwise
	D3D11_BLEND_DESC bm;
	memset(&bm, 0, sizeof(bm));
	bm.RenderTarget[0].BlendEnable = true;
	bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = Device->CreateBlendState(&bm, &mBlendState);
}

Material::~Material()
{
	SAFE_RELEASE(mSamplerState);
	SAFE_RELEASE(mRasterizer);
	SAFE_RELEASE(mDepthState);
	SAFE_RELEASE(mBlendState);
}

MaterialManager::MaterialManager()
{
	mShaderManager = new ShaderManager;
	InitialiseBaseMaterial();
}

MaterialManager::~MaterialManager()
{
	std::map<std::string, Material*>::iterator it;
	for (it = mMaterialArray.begin(); it != mMaterialArray.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	mMaterialArray.clear();
}

void MaterialManager::InitialiseBaseMaterial()
{
	for (int i = 0; i < CutomShader; i++)
	{
		Material* Mat = new Material;
		Mat->SetShader(mShaderManager->GetShaderByType(BaseShader(i)));
		mMaterialArray[StandardMaterialName[i]] = Mat;
	}
}

ShaderManager* MaterialManager::GetShaderManager() const
{
	return mShaderManager;
}

Material* MaterialManager::CreateMaterial(std::string Name, BaseShader BS)
{
	if (mMaterialArray.find(Name) != mMaterialArray.end())
	{
		return mMaterialArray[Name];
	}
	Material* Mat = new Material;
	if (BS != CutomShader)
	{
		Mat->SetShader(mShaderManager->GetShaderByType(BS));
	}
	mMaterialArray[Name] = Mat;

	return Mat;
}

Material* MaterialManager::GetMaterialByName(std::string Name)
{
	if (mMaterialArray.find(Name) == mMaterialArray.end())
	{
		return nullptr;
	}
	return mMaterialArray[Name];
}

Material* MaterialManager::GetMaterialByShaderType(BaseShader BS)
{
	std::string Name = StandardMaterialName[BS];
	return GetMaterialByName(Name);
}