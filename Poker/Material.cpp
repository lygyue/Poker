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
Material::Material(std::string Name)
{
	memset(mConstBuffer, 0, sizeof(mConstBuffer));
	mConstBufferLen = 0;
	mMainTextureIndex = 0;
	mTextureCount = 0;
	for (int i = 0; i < MaxTexture; i++)
	{
		mTex[i] = nullptr;
	}	
	mShader = nullptr;
	mName = Name;
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

char* Material::GetConstBufferPointer()
{
	return mConstBuffer;
}

void Material::SetConstBufferLen(int Len)
{
	mConstBufferLen = Len;
}

int Material::GetConstBufferLen() const
{
	return mConstBufferLen;
}

std::string Material::GetName() const
{
	return mName;
}

Shader* Material::GetShader() const
{
	return mShader;
}

void Material::SetShader(Shader* S)
{
	mShader = S;
}

void Material::Reset()
{
	for (int i = 0; i < mTextureCount; i++)
	{
		mTex[i] = nullptr;
	}
	mTextureCount = 0;
}

int Material::GetTextureCount() const
{
	return mTextureCount;
}

D3d11Texture* Material::GetTexture(int TextureIndex) const
{
	return mTex[TextureIndex];
}

void Material::SetTexture(D3d11Texture* Tex, int TextureIndex /* = 0 */)
{
	if (TextureIndex >= MaxTexture)
	{
		return;
	}
	if (mTextureCount < TextureIndex + 1)
	{
		mTextureCount = TextureIndex + 1;
	}
	mTex[TextureIndex] = Tex;
}

void Material::SetMainTextureIndex(int TextureIndex)
{
	mMainTextureIndex = TextureIndex;
}

int Material::GetMainTextureIndex() const
{
	return mMainTextureIndex;
}

D3d11Texture* Material::GetMainTexture() const
{
	return mTex[mMainTextureIndex];
}

MaterialManager::MaterialManager()
{
	mShaderManager = new ShaderManager;
	mCurrentMaterialIndex = 0;
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

void MaterialManager::Initialise()
{
	mShaderManager->InitialiseStandardShaders();
	for (int i = 0; i < CutomShader; i++)
	{
		std::string Name = StandardMaterialName[i];
		Material* Mat = new Material(Name);
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
	Material* Mat = new Material(Name);
	if (BS != CutomShader)
	{
		Mat->SetShader(mShaderManager->GetShaderByType(BS));
	}
	mMaterialArray[Name] = Mat;
	mCurrentMaterialIndex++;
	return Mat;
}

Material* MaterialManager::CreateMaterial(BaseShader BS)
{
	char szTemp[128];
	memset(szTemp, 0, 128);
	sprintf_s(szTemp, 128, "%s%d", "Material_Auto_Generate_Name_", mCurrentMaterialIndex);
	return CreateMaterial(szTemp, BS);
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

void MaterialManager::DestroyMaterial(std::string Name)
{
	if (mMaterialArray.find(Name) == mMaterialArray.end())
	{
		return;
	}

	Material* M = mMaterialArray[Name];
	SAFE_DELETE(M);
	mMaterialArray.erase(Name);
	return;
}