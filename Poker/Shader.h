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
class Shader
{
	friend class ShaderManager;
public:

protected:
	Shader();
	~Shader();
private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
};

class ShaderManager
{
public:
	static ShaderManager* GetInstance();

	Shader* CreateShader(std::string VSD, std::string PSD);
	Shader* GetShaderByName(std::string Name);
	Shader* GetShaderByType(BaseShader BS);
protected:
	ShaderManager();
	~ShaderManager();
private:

};