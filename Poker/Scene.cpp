/*!
 * \file Scene.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "Scene.h"

Scene* Scene::CurrentScene = nullptr;
Scene::Scene()
{
	mRootSceneNode = new SceneNode("Root_Scene_Node");
	mRenderSystem = new RenderSystemD3D11;
	mRenderGroupManager = new RenderGroupManager;
	mResourceManager = new ResourceManager;
	mMaterialManager = new MaterialManager;
	mMeshManager = new MeshManager;
	mCamera = new Camera;
	mCamera->SetProjectionParameters(DegreesToRadians(90), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.01f, 100.0f);
}

Scene::~Scene()
{
	SAFE_DELETE(mRootSceneNode);
	SAFE_DELETE(mRenderSystem);
	SAFE_DELETE(mRenderGroupManager);
	SAFE_DELETE(mResourceManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mCamera);
}

Scene* Scene::GetCurrentScene()
{
	return CurrentScene;
}

void Scene::SetCurrentScene(Scene* S)
{
	CurrentScene = S;
}

RenderSystemD3D11* Scene::GetRenderSystem() const
{
	return mRenderSystem;
}

RenderGroupManager* Scene::GetRenderGroupManager() const
{
	return mRenderGroupManager;
}

ResourceManager* Scene::GetResourceManager() const
{
	return mResourceManager;
}

MaterialManager* Scene::GetMaterialManager() const
{
	return mMaterialManager;
}

MeshManager* Scene::GetMeshManager() const
{
	return mMeshManager;
}

TextureManager* Scene::GetTextureManager() const
{
	return mTextureManager;
}

Camera* Scene::GetCurrentCamera() const
{
	return mCamera;
}

SceneNode* Scene::GetRootSceneNode() const
{
	return mRootSceneNode;
}

void Scene::RenderOneFrame()
{
	mRenderSystem->SetAndClearRenderTarget();
	mRenderSystem->SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	mRenderGroupManager->RenderAllQueue();
	mRenderSystem->Present();
}
