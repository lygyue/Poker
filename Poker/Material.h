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
public:

protected:
	Material();
	~Material();

private:
	D3d11Texture            * mTex;
	ID3D11InputLayout       * mInputLayout;
	ID3D11SamplerState      * mSamplerState;
	ID3D11RasterizerState   * mRasterizer;
	ID3D11DepthStencilState * mDepthState;
	ID3D11BlendState        * mBlendState;
};

class MaterialManager
{
public:
	static MaterialManager* GetInstance();

	Material* CreateMaterial(std::string Name);
	Material* GetMaterialByName(std::string Name) const;
	Material* GetMaterialByShaderType(BaseShader BS) const;
protected:
	MaterialManager();
	~MaterialManager();
private:

};
