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
"float4x4 ProjViewWorld;"
"void main(in  float4 Position  : POSITION,"
"          out float4 oPosition : SV_Position)"
"{   oPosition = mul(ProjViewWorld, Position);}";

static char* DefaultStandardSampleVertexShaderSrc =
"float4x4 ProjViewWorld;"
"void main(in  float4 Position  : POSITION,    in  float2 TexCoord : TEXCOORD0,"
"          out float4 oPosition : SV_Position, out float2 oTexCoord : TEXCOORD0)"
"{   oPosition = mul(ProjViewWorld, Position);"
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


static char* DefaultPixelShaderSrcSimpleSample =
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{   float4 TexCol = Texture.Sample(Linear, TexCoord); "
//"    if (TexCol.a==0) clip(-1); " // If alpha = 0, don't draw
"    return TexCol;}";

static char* DefaultPixelShaderSrcSimpleFade =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexCol = Texture.Sample(Linear, TexCoord); "
"	TexCol = TexCol * Alpha;"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleFadeInOut =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D TextureOut   : register(t0); SamplerState Linear : register(s0); "
"Texture2D TextureIn   : register(t1); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexColIn = TextureIn.Sample(Linear, TexCoord); "
"	float4 TexColOut = TextureOut.Sample(Linear, TexCoord); "
"	float4 TexCol = TexColIn * Alpha + TexColOut * (1 - Alpha);"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleN_B_N =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexCol = Texture.Sample(Linear, TexCoord); "
"	TexCol = saturate((1 - TexCol) * Alpha + TexCol);"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleL_R_L =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexCol = Texture.Sample(Linear, TexCoord); "
"	if(TexCoord.x < Alpha) TexCol.a = 0;"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleElipseScale =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexCol = Texture.Sample(Linear, TexCoord); "
"	float2 Coord = TexCoord * 2.0 - 1.0;"			// extern to -1, 1
"	Coord.x = Coord.x * 16.0 / 9.0;"				// extern to the real resolution
"	float AntiAlpha = 1.0 - Alpha;"
"	float a = AntiAlpha * 1.5 * 16.0 / 9.0;"
"	float c = a / 2.0;"
"	float2 c1 = float2(c, 0);"
"	float2 c2 = float2(c * -1.0, 0);"
"	float d = distance(c1, Coord) + distance(c2, Coord);"
"	if(d > a * 2) TexCol.a = 0;"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleLayerAlpha =
"Texture2D TextureOut   : register(t0); SamplerState Linear : register(s0); "
"Texture2D TextureIn   : register(t1); "
"Texture2D TextureAlpha   : register(t2); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 TexColIn = TextureIn.Sample(Linear, TexCoord); "
"	float4 TexColOut = TextureOut.Sample(Linear, TexCoord); "
"	float Alpha = TextureAlpha.Sample(Linear, TexCoord).r;"
"	float4 TexCol = TexColIn * Alpha + TexColOut * (1 - Alpha);"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleHelix =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D Texture   : register(t0); SamplerState Linear : register(s0); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float2 Coord = TexCoord * 2.0 - 1.0;"
"	float Current_R = length(Coord);"
"	float AntiAlpha = 1.0 - Alpha;"
"	float Original_R = 0.0;"
"	if(AntiAlpha > 0)"
"	{"
"	Original_R = Current_R / AntiAlpha;"
"	}"
"	float Circles = 1.0;"
"	float PI = 3.141592654;"
"	float CurrentAngle = atan2(Coord.y, Coord.x);"
"	float RotateAngle = Circles * 2.0 * PI * Alpha;"
"	float OriginalAngle = CurrentAngle + RotateAngle;"
"	float x = Original_R * cos(OriginalAngle);"
"	float y = Original_R * sin(OriginalAngle);"
"	float4 TexCol;"
"	if(x > 1 || x < -1 || y > 1 || y < -1)"
"	{"
"	TexCol = float4(0, 0, 0, 0);"
"	}"
"	else"
"	{"
"	Coord = float2(x, y);"
"	Coord = (Coord + 1.0) / 2.0;"
"	TexCol = Texture.Sample(Linear, Coord);"
"	TexCol = saturate(TexCol * (1 + Alpha));"
"	}"
"	float4 OriginalTex = Texture.Sample(Linear, TexCoord);"
"	TexCol = TexCol * Alpha + OriginalTex * AntiAlpha;"
"	return TexCol;"
"}";

static char* DefaultPixelShaderSrcSimpleLighting =
"cbuffer SceneConstantBuffer : register(b0)"
"{"
"	float4x4 ProjViewWorld;"
"	float Alpha;"
"}"
"Texture2D TextureOut   : register(t0); SamplerState Linear : register(s0); "
"Texture2D TextureIn   : register(t1); "
"float4 main(in float4 Position : SV_Position, in float2 TexCoord : TEXCOORD0) : SV_Target"
"{"
"	float4 LightCol = float4(0.58, 0.157, 1, 1);"
"	float3 LightDirStart = normalize(float3(-1, -1, 1));"
"	float3 LightDirEnd = normalize(float3(1, -1, 1));"
"	float4 SceneAmbient = float4(0.5, 0.5, 0.5, 1);"
"	float4 TexColIn = TextureIn.Sample(Linear, TexCoord); "
"	float4 TexColOut = TextureOut.Sample(Linear, TexCoord); "
"	float3 surfaceNormal = normalize(TexColOut.rgb * 2.0 - 1.0);"
"	float3 surfaceNormal_In = normalize(TexColIn.rgb * 2.0 - 1.0);"
"	float3 L =  LightDirStart + (LightDirEnd - LightDirStart) * Alpha;"
//"	float3 L =  LightDirStart;"
"	float diffuseAmount = saturate(dot(L, surfaceNormal));"
"	float diffuseAmount_In = saturate(dot(L, surfaceNormal_In));"
"	float3 diffuse = diffuseAmount * LightCol.rgb * TexColOut.rgb;"
"	float3 diffuse_In = diffuseAmount_In * LightCol.rgb * TexColIn.rgb;"
"	float AssumeDepth = -3.0;"
"	float2 WorldXY = TexCoord * 2.0 - 1.0;"
//"	LightCol = LightCol * (1 - Alpha);"
"	float3 R = reflect(-L, surfaceNormal);"
"	float3 R_In = reflect(-L, surfaceNormal_In);"
"	float3 ViewDir = -float3(WorldXY, AssumeDepth);"
"	float RdotV = dot(R, ViewDir);"
"	float RdotV_In = dot(R_In, ViewDir);"
"	float3 Specular = LightCol * pow(max(0.001f, RdotV), 8.0);"
"	float3 Specular_In = LightCol * pow(max(0.001f, RdotV_In), 8.0);"
"	float4 TexCol = (SceneAmbient + float4(diffuse, 1.0)) * TexColOut + float4(Specular, 1.0);"
"	float4 TexCol_In = (SceneAmbient + float4(diffuse_In, 1.0)) * TexColIn + float4(Specular_In, 1.0);"
"	TexCol = TexCol * TexCol_In;"
"	TexCol = TexColOut * (1.0 - Alpha) + TexCol * Alpha;"
"	return TexCol;"
"}";

std::string StandardShaderName[CutomShader] = { "Simple_Black", "Simple_White", "Simple_Red", "Simple_Green", "Simple_Blue", "Simple_Texture_Sample" ,
"Simple_Fade","Simple_Fade_In_Out", "Simple_N_B_N", "Simple_L_R_L", "Simple_Elipse_Scale", "Simple_Layer_Alpha", "Simple_Helix", "SimpleLighting" };

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

ShaderManager::ShaderManager()
{

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
	CreateCustomShader(StandardShaderName[SimpleFade], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleFade, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleFadeInOut], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleFadeInOut, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleN_B_N], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleN_B_N, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleL_R_L], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleL_R_L, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleElipseScale], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleElipseScale, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleLayerAlpha], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleLayerAlpha, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleHelix], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleHelix, ShaderElementFlag);
	CreateCustomShader(StandardShaderName[SimpleLighting], DefaultStandardSampleVertexShaderSrc, DefaultPixelShaderSrcSimpleLighting, ShaderElementFlag);
}

Shader* ShaderManager::CreateCustomShader(std::string Name, std::string VSD, std::string PSD, unsigned int ShaderElementFlag)
{
	if (mShaderArray.find(Name) != mShaderArray.end())
	{
		return mShaderArray[Name];
	}
	Shader* S = new Shader;
	bool ret = S->Initialise(Scene::GetCurrentScene()->GetRenderSystem()->GetD3d11Device(), VSD, PSD, ShaderElementFlag);

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

void ShaderManager::DestroyShader(std::string Name)
{
	if (mShaderArray.find(Name) != mShaderArray.end())
	{
		Shader* S = mShaderArray[Name];
		SAFE_DELETE(S);
		mShaderArray.erase(Name);
	}
	return;
}