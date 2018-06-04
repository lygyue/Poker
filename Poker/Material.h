/*!
 * \file Material.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Texture.h"
#include "Shader.h"

class MaterialManager;
class Material
{
	friend class MaterialManager;
	friend class Mesh;
public:
	void SetTexture(D3d11Texture* Tex)
	{
		mTex = Tex;
	}
	D3d11Texture* GetTexture() const
	{
		return mTex;
	}
	void SetShader(Shader* S)
	{
		mShader = S;
	}
	Shader* GetShader() const
	{
		return mShader;
	}
protected:
	Material();
	~Material();

	D3d11Texture            * mTex;
	Shader					* mShader;
	ID3D11SamplerState      * mSamplerState;
	ID3D11RasterizerState   * mRasterizer;
	ID3D11DepthStencilState * mDepthState;
	ID3D11BlendState        * mBlendState;
};

class MaterialManager
{
	friend class Scene;
public:
	ShaderManager* GetShaderManager() const;
	// if BS == CutomShader£¬you must set your custom shader by calling SetShader() function.
	Material* CreateMaterial(std::string Name, BaseShader BS);
	Material* GetMaterialByName(std::string Name);
	Material* GetMaterialByShaderType(BaseShader BS);
protected:
	MaterialManager();
	~MaterialManager();

	void InitialiseBaseMaterial();
private:
	std::map<std::string, Material*> mMaterialArray;
	ShaderManager* mShaderManager;
};
