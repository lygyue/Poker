/*!
 * \file Shader.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "Shader.h"
#include "Scene.h"

static char* DefaultStandardVertexShaderSrc =
"float4x4 ProjView;"
"void main(in  float4 Position  : POSITION,"
"          out float4 oPosition : SV_Position)"
"{   oPosition = mul(ProjView, Position);}";

static char* DefaultStandardSampleVertexShaderSrc =
"float4x4 ProjView;"
"void main(in  float4 Position  : POSITION,    in  float2 TexCoord : TEXCOORD0,"
"          out float4 oPosition : SV_Position, out float2 oTexCoord : TEXCOORD0)"
"{   oPosition = mul(ProjView, Position);"
"	 oTexCoord = TexCoord;}";

static char* DefaultPixelShaderSrcBlack =
"float4 main(in float4 Position : SV_Position) : SV_Target"
"{ "
"    return float4(0, 0, 0, 1); }";

static char* DefaultPixelShaderSrcWhite =
"float4 main(in float4 Position : SV_Position) : SV_Target"
"{ "
"    return float4(1, 1, 1, 1); }";

static char* DefaultPixelShaderSrcRed =
"float4 main(in float4 Position : SV_Position) : SV_Target"
"{ "
"    return float4(1, 0, 0, 1); }";

static char* DefaultPixelShaderSrcGreen =
"float4 main(in float4 Position : SV_Position) : SV_Target"
"{ "
"    return float4(0, 1, 0, 1); }";

static char* DefaultPixelShaderSrcBlue =
"float4 main(in float4 Position : SV_Position) : SV_Target"
"{ "
"    return float4(0, 0, 1, 1); }";


char* DefaultPixelShaderSrcSimpleSample =
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{   float4 TexCol = Texture.Sample(Linear, TexCoord); "
"    if (TexCol.a==0) clip(-1); " // If alpha = 0, don't draw
"    return TexCol;}";

Shader::Shader()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mInputLayout = nullptr;
}

Shader::~Shader()
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mPixelShader);
	SAFE_RELEASE(mInputLayout);
}

bool Shader::Initialise(ID3D11Device* Device, std::string VSD, std::string PSD, unsigned int ShaderElementFlag)
{
	D3D11_INPUT_ELEMENT_DESC ElementDesc[10];
	UINT ElePos = 0;
	UINT Offset = 0;
	if (ShaderElementFlag & Ele_Position)
	{
		ElementDesc[ElePos++] = { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 12;
	}
	if (ShaderElementFlag & Ele_TexCoord0)
	{
		ElementDesc[ElePos++] = { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 8;
	}
	if (ShaderElementFlag & Ele_TexCoord1)
	{
		ElementDesc[ElePos++] = { "TexCoord", 1, DXGI_FORMAT_R32G32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 8;
	}
	if (ShaderElementFlag & Ele_TexCoord2)
	{
		ElementDesc[ElePos++] = { "TexCoord", 2, DXGI_FORMAT_R32G32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 8;
	}
	if (ShaderElementFlag & Ele_Normal)
	{
		ElementDesc[ElePos++] = { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 12;
	}
	if (ShaderElementFlag & Ele_Color)
	{
		ElementDesc[ElePos++] = { "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		Offset += 12;
	}
	int numVertexDesc = ElePos;
	// Create vertex shader
	ID3DBlob * blobData;
	ID3DBlob * errorBlob = nullptr;
	HRESULT result = D3DCompile(VSD.c_str(), VSD.length(), 0, 0, 0, "main", "vs_4_0", 0, 0, &blobData, &errorBlob);
	if (FAILED(result))
	{
		return false;
	}
	result = Device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// Create input layout
	result = Device->CreateInputLayout(ElementDesc, numVertexDesc,
		blobData->GetBufferPointer(), blobData->GetBufferSize(), &mInputLayout);
	blobData->Release();
	if (FAILED(result))
	{
		return false;
	}
	// Create pixel shader
	result = D3DCompile(PSD.c_str(), PSD.length(), 0, 0, 0, "main", "ps_4_0", 0, 0, &blobData, 0);
	result = Device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &mPixelShader);
	blobData->Release();
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

std::string StandardShaderName[CutomShader] = { "Simple_Black", "Simple_White", "Simple_Red", "Simple_Green", "Simple_Blue", "Simple_Texture_Sample" };
ShaderManager::ShaderManager()
{
	InitialiseStandardShaders();
}

ShaderManager::~ShaderManager()
{
	std::map<std::string, Shader*>::iterator it;
	for (it = mShaderArray.begin(); it != mShaderArray.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	mShaderArray.clear();
}

void ShaderManager::InitialiseStandardShaders()
{
	unsigned int ShaderElementFlag = 0;
	// simple black
	ShaderElementFlag |= Ele_Position;
	CreateCustomShader(StandardShaderName[SimpleBlack], DefaultStandardVertexShaderSrc, DefaultPixelShaderSrcBlack, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleWhite], DefaultStandardVertexShaderSrc, DefaultPixelShaderSrcWhite, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleRed], DefaultStandardVertexShaderSrc, DefaultPixelShaderSrcRed, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleGreen], DefaultStandardVertexShaderSrc, DefaultPixelShaderSrcGreen, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleBlue], DefaultStandardVertexShaderSrc, DefaultPixelShaderSrcBlue, ShaderElementFlag);

	ShaderElementFlag |= Ele_TexCoord0;
	CreateCustomShader(StandardShaderName[SimpleTextureSample], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleSample, ShaderElementFlag);
}

Shader* ShaderManager::CreateCustomShader(std::string Name, std::string VSD, std::string PSD, unsigned int ShaderElementFlag)
{
	if (mShaderArray.find(Name) != mShaderArray.end())
	{
		return mShaderArray[Name];
	}
	Shader* S = new Shader;
	bool ret = S->Initialise(Scene::GetCurrentScene()->GetCurrentScene()->GetRenderSystem()->GetD3d11Device(), VSD, PSD, ShaderElementFlag);

	if (ret == false)
	{
		SAFE_DELETE(S);
		return nullptr;
	}

	mShaderArray[Name] = S;
	return S;
}

Shader* ShaderManager::GetShaderByName(std::string Name)
{
	if (mShaderArray.find(Name) != mShaderArray.end())
	{
		return mShaderArray[Name];
	}
	return nullptr;
}

Shader* ShaderManager::GetShaderByType(BaseShader BS)
{
	std::string Name = StandardShaderName[BS];
	return GetShaderByName(Name);
}