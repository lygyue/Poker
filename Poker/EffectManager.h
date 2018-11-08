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
#include <list>
#include <map>
#include <xstring>
#include "Math/SimpleMath.h"
#include "Math/Vector3.h"

enum Effect_Type
{
	Effect_FogSimulation,
	Effect_UScroll,
	Effect_PerlinNoiseInOut,
	Effect_Shutter,
	Effect_InOutAndBlurBlend,
	Effect_Simple_Lighting,
	Effect_GPU_Rotate_Helix,
	Effect_Layer_Alpha,
	Effect_Elipse_Scale,
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
protected:
	EffectFadeOut();
	virtual ~EffectFadeOut();

	virtual void Update() override;
};
//-----------------------------------------------------------------------
class EffectFadeInOut : public Effect
{
	friend class EffectManager;
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
enum BaseShader;
class EffectFadeInOutBlend : public Effect
{
	friend class EffectManager;
protected:
	EffectFadeInOutBlend();
	virtual ~EffectFadeInOutBlend();

	void SetShaderType(BaseShader BS)
	{
		mShaderType = BS;
	}

	virtual void Initialise() override;
	virtual void Update() override;

	BaseShader mShaderType;
};
//-----------------------------------------------------------------------
class EffectN_B_N : public Effect
{
	friend class EffectManager;
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
protected:
	Effect_RotateOutIn();
	virtual ~Effect_RotateOutIn();

	virtual void Update() override;

};
//-----------------------------------------------------------------------
class SimpleSpline;
class Effect_SeparateTile : public Effect
{
	friend class EffectManager;
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
class EffectShutter : public Effect
{
	struct SimpleAnimationStruct
	{
		float StartTime;
		float EndTime;
		float CurrentTime;
		int Circles;
		Vector3 RotateVector;
	};
	friend class EffectManager;
protected:
	EffectShutter();
	virtual ~EffectShutter();

	void SetTile(int X, int Y);
	virtual void Initialise() override;
	virtual void Update() override;
private:
	int mTileX;
	int mTileY;
	SceneNode* mAnimationRoot;
	std::vector<Mesh*> mAnimationMeshArray;
	std::vector<SceneNode*> mAnimationNodeArray;
	std::vector<Material*> mMaterialArray;
	std::vector<SimpleAnimationStruct*> mAnimationStructArray;
};
//-----------------------------------------------------------------------
class EffectLayerAlpha : public Effect
{
	friend class EffectManager;
protected:
	EffectLayerAlpha();
	virtual ~EffectLayerAlpha();

	virtual void Initialise() override;
	virtual void Update() override;
private:
	int mAlphaTextureWidth;
	int mAlphaTextureHeight;
	int mBeenSetPixels;
	std::list<int> mAlphaPixelArray;
	Material* mCurrentMaterial;
	unsigned char* mAlphaTextureData;
};
//-----------------------------------------------------------------------
class EffectSimpleLighting : public Effect
{
	friend class EffectManager;
protected:
	EffectSimpleLighting();
	virtual ~EffectSimpleLighting();

	virtual void Initialise() override;
	virtual void Update() override;
private:
	bool mSwitchTexture;
};
//-----------------------------------------------------------------------

class EffectPerlinNoiseInOut : public Effect
{
	friend class EffectManager;
protected:
	EffectPerlinNoiseInOut();
	virtual ~EffectPerlinNoiseInOut();
	virtual void Initialise() override;
	virtual void Update() override;

	bool mSwitchTexture;
};
//-----------------------------------------------------------------------
class EffectFogSimulation : public Effect
{
	friend class EffectManager;
protected:
	EffectFogSimulation();
	virtual ~EffectFogSimulation();

	virtual void Initialise() override;
	virtual void Update() override;
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