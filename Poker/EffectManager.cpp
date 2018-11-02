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
#include "Math/Vector2.h"
#include "Functional.h"

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
	mShaderType = SimpleFadeInOut;
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
	Material* CurrentMaterial = MatMgr->CreateMaterial(mShaderType);
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
	Material* Mat = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	MatMgr->DestroyMaterial(Mat->GetName());
	mAttachMesh->SetMaterial(mOriginalMaterial);
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
Effect_SeparateTile::Effect_SeparateTile()
{
	mTileX = mTileY = 4;
	mAnimationRoot = nullptr;
}

Effect_SeparateTile::~Effect_SeparateTile()
{
	if (mAnimationRoot)
	{
		Scene::GetCurrentScene()->GetRootSceneNode()->RemoveAndDestroyChild(mAnimationRoot);
		mAnimationRoot = nullptr;
	}
	Material* Mat = mAnimationMeshArray[0]->GetMaterial();
	D3d11Texture* Tex = mOriginalMaterial->GetTexture(0);
	Scene::GetCurrentScene()->GetTextureManager()->DestroyTexture(Tex);
	mOriginalMaterial->SetTexture(Mat->GetTexture(0));
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	MatMgr->DestroyMaterial(Mat->GetName());
	for (size_t i = 0; i < mAnimationMeshArray.size(); i++)
	{
		Scene::GetCurrentScene()->GetMeshManager()->DestroyMesh(mAnimationMeshArray[i]);
	}
	mAnimationMeshArray.clear();
	for (size_t i = 0; i < mAnimationSplineArray.size(); i++)
	{
		SAFE_DELETE(mAnimationSplineArray[i]);
	}
	mAnimationSplineArray.clear();
}

void Effect_SeparateTile::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	int Tiles = mTileX * mTileY;
	// create root node
	mAnimationRoot = Scene::GetCurrentScene()->GetRootSceneNode()->CreateChild("Effect_SeparateTile_Animation_Root_Node", Vector3::ZERO, Quaternion::IDENTITY, Vector3(1, 1, 1));
	// create meshes and scene nodes
	float xStep = 2.0f / float(mTileX);
	float yStep = 2.0f / float(mTileY);
	float uStep = 1.0f / float(mTileX);
	float vStep = 1.0f / float(mTileY);
	float NodeDepth = FAR_PLANE - 15.0f;
	char szName[128];
	// create the new material
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleTextureSample);
	if (!mNextTexturePath.empty())
	{
		TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
		RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
		D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);
		CurrentMaterial->SetTexture(Tex);
	}
	for (int i = 0; i < mTileY; i++)
	{
		for (int j = 0; j < mTileX; j++)
		{
			memset(szName, 0, sizeof(szName));
			sprintf_s(szName, 128, "%s_%d_%d", "Effect_SeparateTile_Animation_Root_Node_Child", j, i);
			SceneNode* Node = mAnimationRoot->CreateChild(szName, Vector3::ZERO, Quaternion::IDENTITY, Vector3(1, 1, 1));
			mAnimationNodeArray.push_back(Node);
			float xCenter = (xStep / 2.0f) + j * xStep - 1.0f;
			float yCenter = (yStep / 2.0f) + i * yStep - 1.0f;
			mAnimationSplineArray.push_back(CreateSimpleSpline(Vector3(xCenter, yCenter, NodeDepth), NodeDepth));
			Vector3 Pos[4];
			Vector2 UV[4];
			Pos[0] = Vector3(-xStep / 2.0f, yStep / 2.0f, 0);
			Pos[1] = Vector3(xStep / 2.0f, yStep / 2.0f, 0);
			Pos[2] = Vector3(xStep / 2.0f, -yStep / 2.0f, 0);
			Pos[3] = Vector3(-xStep / 2.0f, -yStep / 2.0f, 0);
			UV[0] = Vector2(uStep * j, 1.0f - vStep * (i + 1));
			UV[1] = Vector2(uStep * (j + 1), 1.0f - vStep * (i + 1));
			UV[2] = Vector2(uStep * (j + 1), 1.0f - vStep * i);
			UV[3] = Vector2(uStep * j, 1.0f - vStep * i);
			Mesh* M = Scene::GetCurrentScene()->GetMeshManager()->CreateQuad(szName, Pos, UV);
			M->SetMaterial(CurrentMaterial);
			Node->AttachMesh(M);
			mAnimationMeshArray.push_back(M);
			mAnimationRandomCircleArray.push_back(RangeRandom(0, mTotalTime));
			NodeDepth--;
		}
	}
}

SimpleSpline* Effect_SeparateTile::CreateSimpleSpline(Vector3& DestPosition, float DestDepth) const
{
	SimpleSpline* SSP = new SimpleSpline;
	float x, y;
	x = RangeRandom(-5.0f, 5.0f);
	y = RangeRandom(-5.0f, 5.0f);
	if (x <= 0)
	{
		x -= 1.0f;		// (-2 .-1) range
	}
	else
	{
		x += 1.0f;		// (1, 2) range
	}
	if (y <= 0)
	{
		y -= 1.0f;
	}
	else
	{
		y += 1.0f;
	}
	// first point
	SSP->addPoint(Vector3(x, y, DestDepth));
	x = RangeRandom(-1.0f, 1.0f);
	y = RangeRandom(-1.0f, 1.0f);
	// second point
	SSP->addPoint(Vector3(x, y, DestDepth));
	// third point, the screen center
	SSP->addPoint(Vector3(0, 0, DestDepth));
	// fourth point
	SSP->addPoint(DestPosition);

	return SSP;
}

void Effect_SeparateTile::SetTile(int X, int Y)
{
	mTileX = X;
	mTileY = Y;
}

void Effect_SeparateTile::Update()
{
	float Alpha = CalculateCurrentAlpha();

	for (size_t i = 0; i < mAnimationSplineArray.size(); i++)
	{
		float CurrentRollRadians = (PI * 2) * mAnimationRandomCircleArray[i] * Alpha * -1.0f;
		Quaternion q;
		q.FromAngleAxis(Radian(CurrentRollRadians), Vector3::UNIT_Z);
		mAnimationNodeArray[i]->SetRotation(q);
		Vector3 pos = mAnimationSplineArray[i]->interpolate(Alpha);
		mAnimationNodeArray[i]->SetPosition(pos);
	}
}
//-----------------------------------------------------------------------
EffectShutter::EffectShutter()
{
	mTileY = 32;
	mTileX = mTileY * 16 / 9;
	mAnimationRoot = nullptr;
}

EffectShutter::~EffectShutter()
{
	if (mAnimationRoot)
	{
		Scene::GetCurrentScene()->GetRootSceneNode()->RemoveAndDestroyChild(mAnimationRoot);
		mAnimationRoot = nullptr;
	}
	mAttachMesh->SetVisible(true);
	Material* Mat = mAnimationMeshArray[0]->GetMaterial();
	D3d11Texture* Tex = mOriginalMaterial->GetTexture(0);
	Scene::GetCurrentScene()->GetTextureManager()->DestroyTexture(Tex);
	mOriginalMaterial->SetTexture(Mat->GetTexture(0));
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	for (size_t i = 0; i < mAnimationMeshArray.size(); i++)
	{
		Scene::GetCurrentScene()->GetMeshManager()->DestroyMesh(mAnimationMeshArray[i]);
	}
	mAnimationMeshArray.clear();
	for (size_t i = 0; i < mMaterialArray.size(); i++)
	{
		Scene::GetCurrentScene()->GetMaterialManager()->DestroyMaterial(mMaterialArray[i]->GetName());
	}
	mMaterialArray.clear();
	for (size_t i = 0; i < mAnimationStructArray.size(); i++)
	{
		SAFE_DELETE(mAnimationStructArray[i]);
	}
	mAnimationStructArray.clear();
	// not need to destroy, will be destroy from root node.
	mAnimationNodeArray.clear();
}

void EffectShutter::SetTile(int X, int Y)
{
	mTileX = X;
	mTileY = Y;
}

void EffectShutter::Initialise()
{
	mAttachMesh->SetVisible(false);

	mOriginalMaterial = mAttachMesh->GetMaterial();
	int Tiles = mTileX * mTileY;
	// create root node
	mAnimationRoot = Scene::GetCurrentScene()->GetRootSceneNode()->CreateChild("Effect_Shutter_Animation_Root_Node", Vector3::ZERO, Quaternion::IDENTITY, Vector3(1, 1, 1));
	// create meshes and scene nodes
	float xStep = 2.0f / float(mTileX);
	float yStep = 2.0f / float(mTileY);
	float uStep = 1.0f / float(mTileX);
	float vStep = 1.0f / float(mTileY);
	float NodeDepth = FAR_PLANE - 15.0f;
	char szName[128];
	// create the new material
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();

	TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
	RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
	D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);

	for (int i = 0; i < mTileY; i++)
	{
		for (int j = 0; j < mTileX; j++)
		{
			memset(szName, 0, sizeof(szName));
			sprintf_s(szName, 128, "%s_%d_%d", "Effect_SeparateTile_Animation_Root_Node_Child", j, i);
			SceneNode* Node = mAnimationRoot->CreateChild(szName, Vector3(0, 0, NodeDepth), Quaternion::IDENTITY, Vector3(1, 1, 1));
			mAnimationNodeArray.push_back(Node);
			float xCenter = (xStep / 2.0f) + j * xStep - 1.0f;
			float yCenter = (yStep / 2.0f) + i * yStep - 1.0f;

			Vector3 Pos[4];
			Vector2 UV[4];
			Pos[0] = Vector3(-xStep / 2.0f, yStep / 2.0f, 0);
			Pos[1] = Vector3(xStep / 2.0f, yStep / 2.0f, 0);
			Pos[2] = Vector3(xStep / 2.0f, -yStep / 2.0f, 0);
			Pos[3] = Vector3(-xStep / 2.0f, -yStep / 2.0f, 0);
			UV[0] = Vector2(uStep * j, 1.0f - vStep * (i + 1));
			UV[1] = Vector2(uStep * (j + 1), 1.0f - vStep * (i + 1));
			UV[2] = Vector2(uStep * (j + 1), 1.0f - vStep * i);
			UV[3] = Vector2(uStep * j, 1.0f - vStep * i);
			Mesh* M = Scene::GetCurrentScene()->GetMeshManager()->CreateQuad(szName, Pos, UV);
			Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleFadeInOut);
			M->SetMaterial(CurrentMaterial);
			mMaterialArray.push_back(CurrentMaterial);
			Node->AttachMesh(M);
			mAnimationMeshArray.push_back(M);
			SimpleAnimationStruct* SAS = new SimpleAnimationStruct;
			SAS->StartTime = RangeRandom(0, mTotalTime / 2.0f);
			SAS->EndTime = RangeRandom(mTotalTime / 2.0f, mTotalTime);
			SAS->CurrentTime = 0.0f;
			SAS->RotateVector = Vector3(RangeRandom(-1.0f, 1.0f), RangeRandom(-1.0f, 1.0f), 0).normalisedCopy();
			mAnimationStructArray.push_back(SAS);
		}
	}
}

void EffectShutter::Update()
{
	for (size_t i = 0; i < mAnimationStructArray.size(); i++)
	{
		SimpleAnimationStruct* SAS = mAnimationStructArray[i];
		SceneNode* N = mAnimationNodeArray[i];
		Material* Mat = mMaterialArray[i];
		float Alpha = 0.0f;
		SAS->CurrentTime += (Timer::GetInstance()->GetDelta()) * 0.001f;
		if (SAS->CurrentTime > SAS->StartTime)
		{
			Alpha = (SAS->CurrentTime - SAS->StartTime) / (SAS->EndTime - SAS->StartTime);
			if (Alpha > 1) Alpha = 1.0f;

			Radian R = Radian(Alpha * 2 * PI);
			Quaternion q;
			q.FromAngleAxis(R, SAS->RotateVector);
			N->SetRotation(q);
		}
		char* TempPtr = Mat->GetConstBufferPointer();
		memcpy(TempPtr, &Alpha, sizeof(float));
		Mat->SetConstBufferLen(sizeof(float));
	}
}

//-----------------------------------------------------------------------
EffectLayerAlpha::EffectLayerAlpha()
{
	mCurrentMaterial = nullptr;
	mAlphaTextureHeight = 128;
	mAlphaTextureWidth = 128;
	mBeenSetPixels = 0;
	int TotalAlphaPixels = mAlphaTextureHeight * mAlphaTextureWidth;
	mAlphaTextureData = new unsigned char[TotalAlphaPixels];
	memset(mAlphaTextureData, 0, TotalAlphaPixels);
	for (int i = 0; i < TotalAlphaPixels; i++)
	{
		mAlphaPixelArray.push_back(i);
	}
}

EffectLayerAlpha::~EffectLayerAlpha()
{
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	// destroy old texture
	D3d11Texture* Tex = mCurrentMaterial->GetTexture(0);
	TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
	TexMgr->DestroyTexture(Tex);
	// destroy alpha texture
	Tex = mCurrentMaterial->GetTexture(2);
	TexMgr->DestroyTexture(Tex);
	MatMgr->DestroyMaterial(mCurrentMaterial->GetName());
	mCurrentMaterial = nullptr;
	mAttachMesh->SetMaterial(mOriginalMaterial);
	SAFE_DELETE_ARRAY(mAlphaTextureData);
}

void EffectLayerAlpha::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleLayerAlpha);

	TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
	RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
	D3d11Texture* Tex = TexMgr->LoadTextureFromFile(mNextTexturePath, RS->GetD3d11Device(), mNextTexturePath.c_str(), false);
	D3d11Texture* OldTex = mOriginalMaterial->GetTexture(0);
	CurrentMaterial->SetTexture(OldTex, 0);
	CurrentMaterial->SetTexture(Tex, 1);
	mOriginalMaterial->SetTexture(Tex);
	// Create Alpha Texture for blend
	D3d11Texture* AlphaTex = TexMgr->CreateTexture("LayerAlphaTexture", RS->GetD3d11Device(), mAlphaTextureWidth, mAlphaTextureHeight, DXGI_FORMAT_R8_UNORM, 1, true);
	CurrentMaterial->SetTexture(AlphaTex, 2);
	mAttachMesh->SetMaterial(CurrentMaterial);
	mCurrentMaterial = CurrentMaterial;
}

void EffectLayerAlpha::Update()
{
	float Alpha = CalculateCurrentAlpha();
	int TotalAlphaPixels = mAlphaTextureHeight * mAlphaTextureWidth;
	int ALphaPixels = TotalAlphaPixels * Alpha;
	int NeedSetPixels = ALphaPixels - mBeenSetPixels;
	if (NeedSetPixels == 0)
	{
		return;
	}
	mBeenSetPixels = ALphaPixels;
	for (int i = 0; i < NeedSetPixels; i++)
	{
		int pos = RangeRandom(0, float(mAlphaPixelArray.size()) - 0.001);
		std::list<int>::iterator it = mAlphaPixelArray.begin();
		std::advance(it, pos);
		mAlphaTextureData[*it] = 255;
		mAlphaPixelArray.erase(it);
	}
	BYTE* data = NULL;
	UINT pitch = 0;
	D3d11Texture* AlphaTex = mCurrentMaterial->GetTexture(2);
	AlphaTex->BlitToTexture(mAlphaTextureData, TotalAlphaPixels);
}
//-----------------------------------------------------------------------
EffectSimpleLighting::EffectSimpleLighting()
{
	mSwitchTexture = false;
}
EffectSimpleLighting::~EffectSimpleLighting()
{
	Material* Mat = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	// destroy old texture
	D3d11Texture* Tex = Mat->GetTexture(1);
	TextureManager* TexMgr = Scene::GetCurrentScene()->GetTextureManager();
	TexMgr->DestroyTexture(Tex);
	MatMgr->DestroyMaterial(Mat->GetName());
	mAttachMesh->SetMaterial(mOriginalMaterial);
}

void EffectSimpleLighting::Initialise()
{
	mOriginalMaterial = mAttachMesh->GetMaterial();
	MaterialManager* MatMgr = Scene::GetCurrentScene()->GetMaterialManager();
	Material* CurrentMaterial = MatMgr->CreateMaterial(SimpleLighting);
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

void EffectSimpleLighting::Update()
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
			D3d11Texture* Tex0 = mAttachMesh->GetMaterial()->GetTexture(0);
			D3d11Texture* Tex1 = mAttachMesh->GetMaterial()->GetTexture(1);
			mAttachMesh->GetMaterial()->SetTexture(Tex1, 0);
			mAttachMesh->GetMaterial()->SetTexture(Tex0, 1);
		}
	}
	else
	{
		Alpha *= 2;
	}
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
	case Effect_Separate_Tile:
	{
		E = new Effect_SeparateTile;
		break;
	}
	case Effect_Elipse_Scale:
	{
		EffectN_B_N* EF = new EffectN_B_N;
		EF->SetShaderType(SimpleElipseScale);
		E = EF;
		break;
	}
	case Effect_Layer_Alpha:
	{
		EffectLayerAlpha* EF = new EffectLayerAlpha;
		E = EF;
		break;
	}
	case Effect_GPU_Rotate_Helix:
	{
		EffectN_B_N* EF = new EffectN_B_N;
		EF->SetShaderType(SimpleHelix);
		E = EF;
		break;
	}
	case  Effect_Simple_Lighting:
	{
		EffectSimpleLighting* EF = new EffectSimpleLighting;
		E = EF;
		break;
	}
	case Effect_InOutAndBlurBlend:
	{
		EffectFadeInOutBlend* EF = new EffectFadeInOutBlend;
		EF->SetShaderType(SimpleInOutAndBlurBlend);
		E = EF;
		break;
	}
	case Effect_Shutter:
	{
		E = new EffectShutter;
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