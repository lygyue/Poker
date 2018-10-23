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

#define		MAX_CONST_BUFFER		128

class MaterialManager;
class Material
{
	friend class MaterialManager;
	friend class Mesh;
	static const int MaxTexture = 16;
public:
	void SetTexture(D3d11Texture* Tex, int TextureIndex = 0);
	D3d11Texture* GetTexture(int TextureIndex) const;
	void SetMainTextureIndex(int TextureIndex);
	int GetMainTextureIndex() const;
	D3d11Texture* GetMainTexture() const;
	int GetTextureCount() const;
	void Reset();
	void SetShader(Shader* S);
	Shader* GetShader() const;
	std::string GetName() const;
	char* GetConstBufferPointer();
	void SetConstBufferLen(int Len);
	int GetConstBufferLen() const;
protected:
	Material(std::string Name);
	~Material();

	char					  mConstBuffer[MAX_CONST_BUFFER];
	int						  mConstBufferLen;
	std::string				  mName;
	int						  mMainTextureIndex;
	int						  mTextureCount;
	D3d11Texture            * mTex[MaxTexture];
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
	Material* CreateMaterial(BaseShader BS);
	Material* GetMaterialByName(std::string Name);
	Material* GetMaterialByShaderType(BaseShader BS);
	void DestroyMaterial(std::string Name);
protected:
	MaterialManager();
	~MaterialManager();

	void Initialise();
private:
	int mCurrentMaterialIndex;
	std::map<std::string, Material*> mMaterialArray;
	ShaderManager* mShaderManager;
};
