/***********************************************
 * File: EffectManager.cpp
 *
 * Author: LYG
 * Date: Ê®ÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/

#include "EffectManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"
#include "Timer.h"

Effect::Effect()
{
	mAttachMesh = nullptr;
	mAttachSceneNode = nullptr;
	mOriginalMaterial = nullptr;
	mCurrentTime = 0.0f;
	mTotalTime = 0.0f;
	mAcceleration = 0.0f;
	mIsEnd = false;
	mAutoDestroy = true;
	mLoop = false;
}

Effect::~Effect()
{

}

void Effect::SetName(std::string Name)
{
	mName = Name;
}

std::string Effect::GetName() const
{
	return mName;
}

bool Effect::IsEnd() const
{
	return mIsEnd;
}

bool Effect::IsLoop() const
{
	return mLoop;
}

void Effect::SetLoop(bool Loop)
{
	mLoop = Loop;
}

bool Effect::IsAutoDestroy() const
{
	return mAutoDestroy;
}

void Effect::SetAutoDestroy(bool AutoDestroy)
{
	mAutoDestroy = AutoDestroy;
}

void Effect::SetAttachMesh(Mesh* M)
{
	mAttachMesh = M;
}

Mesh* Effect::GetAttachMesh() const
{
	return mAttachMesh;
}

void Effect::SetAttachSceneNode(SceneNode* S)
{
	mAttachSceneNode = S;
}

SceneNode* Effect::GetAttachSceneNode() const
{
	return mAttachSceneNode;
}

float Effect::CalculateCurrentAlpha()
{
	mCurrentTime += (Timer::GetInstance()->GetDelta()) * 0.001f;
	float Alpha = 0.0f;
	if (mCurrentTime >= mTotalTime)
	{
		Alpha = 1.0;
		mIsEnd = true;
	}
	else
	{
		if (RealEqual(mAcceleration, 0.0))
		{
			Alpha = mCurrentTime / mTotalTime;
		}
		else if(mAcceleration > 0)
		{
			float exponential = mAcceleration + 1;
			float a = mCurrentTime / mTotalTime;

			Alpha = pow(a, exponential);
		}
		else
		{
			float a = mCurrentTime / mTotalTime;
			float exponential = 1 / (fabs(mAcceleration) + 1);
			Alpha = pow(a, exponential);
		}
	}
	return Alpha;
}

void Effect::SetAlphaToConstBuffer(float Alpha)
{
	Material* Mat = mAttachMesh->GetMaterial();
	char* TempPtr = Mat->GetConstBufferPointer();
	memcpy(TempPtr, &Alpha, sizeof(float));
	Mat->SetConstBufferLen(sizeof(float));
}
//-----------------------------------------------------------------------
EffectFadeIn::EffectFadeIn()
{

}

EffectFadeIn::~EffectFadeIn()
{
	Material* Mat = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	MatMgr->DestroyMaterial(Mat->GetName());
	mAttachMesh->SetMaterial(mOriginalMaterial);
}

void EffectFadeIn::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleFade);
	if (!mNextTexturePath.empty())
	{
		TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
		RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
		D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);
		D3d11Texture* OldTex = mOriginalMaterial->GetTexture(0);
		TexMgr->DestroyTexture(OldTex);
		CurrentMaterial->SetTexture(Tex);
		mOriginalMaterial->SetTexture(Tex);
	}
	else
	{
		D3d11Texture* OldTex = mOriginalMaterial->GetTexture(0);
		CurrentMaterial->SetTexture(OldTex);
	}
	mAttachMesh->SetMaterial(CurrentMaterial);
}

void EffectFadeIn::Update()
{
	float Alpha = CalculateCurrentAlpha();
	SetAlphaToConstBuffer(Alpha);
	return;
}
//-----------------------------------------------------------------------
EffectFadeOut::EffectFadeOut()
{
	
}

EffectFadeOut::~EffectFadeOut()
{

}

void EffectFadeOut::Update()
{
	float Alpha = 1.0f - CalculateCurrentAlpha();
	SetAlphaToConstBuffer(Alpha);
	return;
}
//-----------------------------------------------------------------------
EffectFadeInOut::EffectFadeInOut()
{
	mFirstEffect = nullptr;
	mSecondEffect = nullptr;
}

EffectFadeInOut::~EffectFadeInOut()
{
	SAFE_DELETE(mSecondEffect);
}

void EffectFadeInOut::Initialise()
{
	mFirstEffect = new EffectFadeOut();
	mFirstEffect->mTotalTime = mTotalTime * 0.5f;
	mFirstEffect->mAcceleration = mAcceleration;
	mFirstEffect->mLoop = mLoop;
	mFirstEffect->mAutoDestroy = mAutoDestroy;
	mFirstEffect->SetAttachMesh(mAttachMesh);
	mFirstEffect->Initialise();
}

void EffectFadeInOut::Update()
{
	if (mFirstEffect)
	{
		mFirstEffect->Update();
		if (mFirstEffect->IsEnd())
		{
			SAFE_DELETE(mFirstEffect);
			mSecondEffect = new EffectFadeIn;
			mSecondEffect->mTotalTime = mTotalTime * 0.5f;
			mSecondEffect->mAcceleration = mAcceleration;
			mSecondEffect->mLoop = mLoop;
			mSecondEffect->mAutoDestroy = mAutoDestroy;
			mSecondEffect->SetAttachMesh(mAttachMesh);
			mSecondEffect->mNextTexturePath = mNextTexturePath;
			mSecondEffect->Initialise();
		}
	}
	else
	{
		mSecondEffect->Update();
	}
}
//-----------------------------------------------------------------------
EffectFadeInOutBlend::EffectFadeInOutBlend()
{

}

EffectFadeInOutBlend::~EffectFadeInOutBlend()
{
	Material* Mat = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	// destroy old texture
	D3d11Texture* Tex = Mat->GetTexture(0);
	TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
	TexMgr->DestroyTexture(Tex);
	MatMgr->DestroyMaterial(Mat->GetName());
	mAttachMesh->SetMaterial(mOriginalMaterial);
}

void EffectFadeInOutBlend::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleFadeInOut);
	if (!mNextTexturePath.empty())
	{
		TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
		RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
		D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);
		D3d11Texture* OldTex = mOriginalMaterial->GetTexture(0);
		CurrentMaterial->SetTexture(OldTex, 0);
		CurrentMaterial->SetTexture(Tex, 1);
		mOriginalMaterial->SetTexture(Tex);
	}
	mAttachMesh->SetMaterial(CurrentMaterial);
}

void EffectFadeInOutBlend::Update()
{
	float Alpha = CalculateCurrentAlpha();
	SetAlphaToConstBuffer(Alpha);
	return;
}
//-----------------------------------------------------------------------
EffectN_B_N::EffectN_B_N()
{
	mSwitchTexture = false;
}

EffectN_B_N::~EffectN_B_N()
{

}

void EffectN_B_N::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(mShaderType);
	CurrentMaterial->SetTexture(mOriginalMaterial->GetTexture(0));
	mAttachMesh->SetMaterial(CurrentMaterial);
	if (!mNextTexturePath.empty())
	{
		TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
		RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
		D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);
		mOriginalMaterial->SetTexture(Tex);
	}
}

void EffectN_B_N::Update()
{
	float Alpha = CalculateCurrentAlpha();
	if (Alpha > 0.5f)
	{
		Alpha -= 0.5f;
		Alpha *= 2;
		Alpha = 1.0f - Alpha;
		if (!mSwitchTexture)
		{
			mSwitchTexture = true;
			D3d11Texture* Tex = mAttachMesh->GetMaterial()->GetTexture(0);
			TextureManager* TextureMgr = Scene::GetCurrentScene()->GetTextureManager();
			TextureMgr->DestroyTexture(Tex);
			mAttachMesh->GetMaterial()->SetTexture(mOriginalMaterial->GetTexture(0));
		}
	}
	else
	{
		Alpha *= 2;
	}
	SetAlphaToConstBuffer(Alpha);
}
//-----------------------------------------------------------------------
Effect_RotateOutIn::Effect_RotateOutIn()
{

}
Effect_RotateOutIn::~Effect_RotateOutIn()
{

}

void Effect_RotateOutIn::Update()
{
	float Alpha = CalculateCurrentAlpha();
	int AssumeCircle = 4;
	float CurrentRollRadians = 0.0f;
	if (Alpha > 0.5f)
	{
		Alpha -= 0.5f;
		Alpha *= 2;
		Alpha = 1.0f - Alpha;
		if (!mSwitchTexture)
		{
			mSwitchTexture = true;
			D3d11Texture* Tex = mAttachMesh->GetMaterial()->GetTexture(0);
			TextureManager* TextureMgr = Scene::GetCurrentScene()->GetTextureManager();
			TextureMgr->DestroyTexture(Tex);
			mAttachMesh->GetMaterial()->SetTexture(mOriginalMaterial->GetTexture(0));
		}
		// anticlockwise
		CurrentRollRadians = (PI * 2) * AssumeCircle * Alpha * -1.0f;
	}
	else
	{
		Alpha *= 2;
		// clockwise
		CurrentRollRadians = (PI * 2) * AssumeCircle * Alpha * -1.0f;
	}
	Quaternion q;
	q.FromAngleAxis(Radian(CurrentRollRadians), Vector3::UNIT_Z);
	mAttachSceneNode->SetRotation(q);
	Vector3 Scale = Vector3(1 - Alpha, 1 - Alpha, 1 - Alpha);
	mAttachSceneNode->SetScale(Scale);
	SetAlphaToConstBuffer(Alpha);
}

//-----------------------------------------------------------------------
EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{

}

Effect* EffectManager::CreateEffect(std::string Name, Effect_Type ET, float TotalTime, bool IsLoop, bool AutoDestroy, float Acceleration, Mesh* M, SceneNode* S /* = nullptr */, const char* NewTexturePath/* = nullptr*/)
{
	if (mEffectArray.find(Name) != mEffectArray.end())
	{
		return mEffectArray[Name];
	}
	Effect* E = nullptr;
	switch (ET)
	{
	case Effect_Fade_In:
	{
		E = new EffectFadeIn();
		break;
	}
	case Effect_Fade_Out:
	{
		E = new EffectFadeOut();
		break;
	}
	case Effect_Fade_In_Out:
	{
		E = new EffectFadeInOut();
		break;
	}
	case Effect_Fade_In_Out_Blend:
	{
		E = new EffectFadeInOutBlend();
		break;
	}
	case  Effect_Norma_Brightness_Normal:
	{
		EffectN_B_N* EF = new EffectN_B_N;
		EF->SetShaderType(SimpleN_B_N);
		E = EF;
		break;
	}
	case Effect_Left_Right_Left:
	{
		EffectN_B_N* EF = new EffectN_B_N;
		EF->SetShaderType(SimpleL_R_L);
		E = EF;
		break;
	}
	case  Effect_Rotate_Out_In:
	{
		Effect_RotateOutIn* EF = new Effect_RotateOutIn;
		EF->SetShaderType(SimpleN_B_N);
		E = EF;
		break;
	}
	default:
		break;
	}
	if (E)
	{
		E->mNextTexturePath = NewTexturePath;
		E->SetName(Name);
		E->mTotalTime = TotalTime;
		E->mLoop = IsLoop;
		E->mAutoDestroy = AutoDestroy;
		E->mAcceleration = Acceleration;
		E->SetAttachMesh(M);
		E->SetAttachSceneNode(S);
		E->Initialise();
		mEffectArray[Name] = E;
	}
	return E;
}

void EffectManager::DestroyEffect(std::string Name)
{
	if (mEffectArray.find(Name) != mEffectArray.end())
	{
		Effect* E = mEffectArray[Name];
		SAFE_DELETE(E);
		mEffectArray.erase(Name);
	}
}

void EffectManager::Update()
{
	std::map<std::string, Effect*>::iterator it;
	for (it = mEffectArray.begin(); it != mEffectArray.end(); )
	{
		if (it->second->IsEnd() && it->second->IsAutoDestroy())
		{
			SAFE_DELETE(it->second);
			it = mEffectArray.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (it = mEffectArray.begin(); it != mEffectArray.end(); it++)
	{
		it->second->Update();
	}
}