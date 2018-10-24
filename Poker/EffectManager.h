/***********************************************
 * File: EffectManager.h
 *
 * Author: LYG
 * Date: Ê®ÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#pragma once
#include <vector>
#include <map>
#include <xstring>
#include "Math/SimpleMath.h"

enum Effect_Type
{
	Effect_Separate_Tile,
	Effect_Rotate_Out_In,
	Effect_Fade_In,
	Effect_Fade_Out,
	Effect_Fade_In_Out,
	Effect_Fade_In_Out_Blend,			// fade in/out at the same time and in/out texture do a blend
	Effect_Norma_Brightness_Normal,
	Effect_Left_Right_Left,
	Effect_Max,
};
//-----------------------------------------------------------------------
class Mesh;
class SceneNode;
class Material;
class Effect
{
	friend class EffectManager;
public:
	void SetName(std::string Name);
	std::string GetName() const;
	bool IsEnd() const;
	bool IsLoop() const;
	void SetLoop(bool Loop);
	bool IsAutoDestroy() const;
	void SetAutoDestroy(bool AutoDestroy);
protected:
	Effect();
	virtual ~Effect();

	void SetAttachMesh(Mesh* M);
	Mesh* GetAttachMesh() const;
	void SetAttachSceneNode(SceneNode* S);
	SceneNode* GetAttachSceneNode() const;
	float CalculateCurrentAlpha();
	void SetAlphaToConstBuffer(float Alpha);

	virtual void Initialise() {}
	virtual void Update() {}

	std::string mNextTexturePath;
	Mesh* mAttachMesh;
	SceneNode* mAttachSceneNode;
	Material* mOriginalMaterial;
	std::string mName;
	float mTotalTime;
	float mCurrentTime;
	float mAcceleration;
	bool mAutoDestroy;
	bool mIsEnd;
	bool mLoop;
};
//-----------------------------------------------------------------------
class EffectFadeIn : public Effect
{
	friend class EffectManager;
	friend class EffectFadeInOut;
public:
protected:
	EffectFadeIn();
	virtual ~EffectFadeIn();

	virtual void Initialise() override;
	virtual void Update() override;
};
//-----------------------------------------------------------------------
class EffectFadeOut : public EffectFadeIn
{
	friend class EffectManager;
	friend class EffectFadeInOut;
public:
protected:
	EffectFadeOut();
	virtual ~EffectFadeOut();

	virtual void Update() override;
};
//-----------------------------------------------------------------------
class EffectFadeInOut : public Effect
{
	friend class EffectManager;
public:
protected:
	EffectFadeInOut();
	virtual ~EffectFadeInOut();

	virtual void Initialise() override;
	virtual void Update() override;

private:
	EffectFadeOut* mFirstEffect;
	EffectFadeIn* mSecondEffect;
};
//-----------------------------------------------------------------------
class EffectFadeInOutBlend : public Effect
{
	friend class EffectManager;
public:
protected:
	EffectFadeInOutBlend();
	virtual ~EffectFadeInOutBlend();

	virtual void Initialise() override;
	virtual void Update() override;
};
//-----------------------------------------------------------------------
enum BaseShader;
class EffectN_B_N : public Effect
{
	friend class EffectManager;
public:
protected:
	EffectN_B_N();
	virtual ~EffectN_B_N();

	void SetShaderType(BaseShader BS)
	{
		mShaderType = BS;
	}

	virtual void Initialise() override;
	virtual void Update() override;

	bool mSwitchTexture;
	BaseShader mShaderType;
};
//-----------------------------------------------------------------------
class Effect_RotateOutIn : public EffectN_B_N
{
	friend class EffectManager;
public:
protected:
	Effect_RotateOutIn();
	virtual ~Effect_RotateOutIn();

	virtual void Update() override;
private:
};
//-----------------------------------------------------------------------
class SimpleSpline;
class Effect_SeparateTile : public Effect
{
	friend class EffectManager;
public:
protected:
	Effect_SeparateTile();
	virtual ~Effect_SeparateTile();

	SimpleSpline* CreateSimpleSpline(Vector3& DestPosition, float DestDepth) const;
	void SetTile(int X, int Y);
	virtual void Initialise() override;
	virtual void Update() override;
private:
	int mTileX;
	int mTileY;
	SceneNode* mAnimationRoot;
	std::vector<Mesh*> mAnimationMeshArray;
	std::vector<SceneNode*> mAnimationNodeArray;
	std::vector<SimpleSpline*> mAnimationSplineArray;
	std::vector<int> mAnimationRandomCircleArray;
};
//-----------------------------------------------------------------------
class EffectManager
{
	friend class Scene;
public:
	Effect* CreateEffect(std::string Name, Effect_Type ET, float TotalTime, bool IsLoop, bool AutoDestroy, float Acceleration, Mesh* M, SceneNode* S = nullptr, const char* NewTexturePath = nullptr);

	void DestroyEffect(std::string Name);
protected:
	EffectManager();
	~EffectManager();

	void Update();
private:
	std::map<std::string, Effect*> mEffectArray;
};