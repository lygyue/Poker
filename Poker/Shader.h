/*!
 * \file Shader.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Common.h"
#include <map>
enum ShaderType
{
	Vertex_Shader,
	Pixel_Shader,
	Geometry_Shader,
};
enum BaseShader
{
	SimpleBlack,
	SimpleWhite,
	SimpleRed,
	SimpleGreen,
	SimpleBlue,
	SimpleTextureSample,
	CutomShader,
};
enum ShaderElement
{
	Ele_Position		= 1 << 0,
	Ele_TexCoord0		= 1 << 1,
	Ele_TexCoord1		= 1 << 2,
	Ele_TexCoord2		= 1 << 3,
	Ele_Normal			= 1 << 4,
	Ele_Color			= 1 << 5,
};
class Shader
{
	friend class ShaderManager;
public:
	ID3D11VertexShader* GetD3d11VertexShader() const
	{
		return mVertexShader;
	}
	ID3D11PixelShader* GetD3d11PixelShader() const
	{
		return mPixelShader;
	}
	ID3D11InputLayout* GetD3d11InputLayout() const
	{
		return mInputLayout;
	}
protected:
	Shader();
	~Shader();

	bool Initialise(ID3D11Device* Device, std::string VSD, std::string PSD, unsigned int ShaderElementFlag);
private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mInputLayout;
};

class ShaderManager
{
	friend class MaterialManager;
public:
	Shader* CreateCustomShader(std::string Name, std::string VSD, std::string PSD, unsigned int ShaderElementFlag);
	Shader* GetShaderByName(std::string Name);
	Shader* GetShaderByType(BaseShader BS);
protected:
	ShaderManager();
	~ShaderManager();

	void InitialiseStandardShaders();
private:
	std::map<std::string, Shader*> mShaderArray;
};