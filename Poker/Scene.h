/*!
 * \file Scene.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Common.h"
#include "RenderSystem.h"
#include "RenderGroupManager.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "SceneNode.h"
#include "PokerDef.h"

class Camera
{
public:
	Camera() 
	{
		mPos = XMVectorSet(0.0f, 0.0f, -5.0f, 0);
		mRot = XMQuaternionIdentity();
		mFovAngle = DegreesToRadians(90);
		mAspect = 1920.0f / 1080.0f;
		mNearPlane = 0.01f;
		mFarPlane = 100.0f;
		mViewMatrix = mProjectionMatrix = mViewProjectionMatrix= XMMatrixIdentity();
	}
	~Camera(){}
	void SetPosition(XMVECTOR Position)
	{
		mPos = Position;
		XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), mRot);
		mViewMatrix = (XMMatrixLookAtLH(mPos, XMVectorAdd(mPos, forward), XMVector3Rotate(XMVectorSet(0, 1, 0, 0), mRot)));
		mViewProjectionMatrix = XMMatrixMultiply(mViewMatrix, mProjectionMatrix);
	}
	void SetRotation(XMVECTOR Rotation)
	{
		mRot = Rotation;
		XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), mRot);
		mViewMatrix = (XMMatrixLookAtLH(mPos, XMVectorAdd(mPos, forward), XMVector3Rotate(XMVectorSet(0, 1, 0, 0), mRot)));
		mViewProjectionMatrix = XMMatrixMultiply(mViewMatrix, mProjectionMatrix);
	}
	void SetProjectionParameters(float Fov, float Aspect, float Near, float Far)
	{
		mFovAngle = Fov;
		mAspect = Aspect;
		mNearPlane = Near;
		mFarPlane = Far;
		mProjectionMatrix = XMMatrixPerspectiveFovLH(mFovAngle, mAspect, mNearPlane, mFarPlane);
		mViewProjectionMatrix = XMMatrixMultiply(mViewMatrix, mProjectionMatrix);
	}
	XMMATRIX GetViewMatrix() const
	{
		return mViewMatrix;
	}
	XMMATRIX GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	XMMATRIX GetProjectViewMatrix() const
	{
		return mViewProjectionMatrix;
	}

	static void* operator new(size_t size)
	{
		UNREFERENCED_PARAMETER(size);
		return _aligned_malloc(sizeof(Camera), __alignof(Camera));
	}

	static void operator delete(void* p)
	{
		_aligned_free(p);
	}
private:
	XMVECTOR mPos;
	XMVECTOR mRot;
	float mFovAngle;
	float mAspect;
	float mNearPlane;
	float mFarPlane;
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;
	XMMATRIX mViewProjectionMatrix;
};

class Scene
{
	friend class PokerLogicManager;
public:
	static Scene* GetCurrentScene();
	static void SetCurrentScene(Scene* S);
public:
	SceneNode* GetRootSceneNode() const;
	RenderSystemD3D11* GetRenderSystem() const;
	RenderGroupManager* GetRenderGroupManager() const;
	ResourceManager* GetResourceManager() const;
	MaterialManager* GetMaterialManager() const;
	MeshManager* GetMeshManager() const;
	TextureManager* GetTextureManager() const;
	Camera* GetCurrentCamera() const;
public:
	void RenderOneFrame();

protected:
	Scene();
	~Scene();
	static Scene* CurrentScene;

private:
	SceneNode* mRootSceneNode;
	RenderSystemD3D11* mRenderSystem;
	RenderGroupManager* mRenderGroupManager;
	ResourceManager* mResourceManager;
	MaterialManager* mMaterialManager;
	MeshManager* mMeshManager;
	TextureManager* mTextureManager;
	Camera* mCamera;
};