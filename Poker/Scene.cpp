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
	mHwnd = nullptr;
	// Get Current Directory
	char Temp[MAX_PATH];
	memset(Temp, 0, MAX_PATH);

	GetModuleFileNameA(NULL, Temp, MAX_PATH);
	int nLen = strlen(Temp);
	while (nLen)
	{
		if (Temp[nLen] == '\\' || Temp[nLen] == '/')
		{
			break;
		}
		Temp[nLen--] = '\0';
	}
	mApplicationPath = Temp;
	mBackGroundNode = nullptr;
	mRootSceneNode = new SceneNode("Root_Scene_Node");
	mRenderSystem = new RenderSystemD3D11;
	mRenderGroupManager = new RenderGroupManager;
	mResourceManager = new ResourceManager;
	mMaterialManager = new MaterialManager;
	mTextureManager = new TextureManager;
	mEffectManager = new EffectManager;
	mMeshManager = new MeshManager;
	mCamera = new Camera;
	mLog = new LogImpl(Temp);
	mCamera->SetProjectionParameters(DegreesToRadians(90), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), NEAR_PLANE, FAR_PLANE);
}

Scene::~Scene()
{
	SAFE_DELETE(mRootSceneNode);
	SAFE_DELETE(mRenderSystem);
	SAFE_DELETE(mRenderGroupManager);
	SAFE_DELETE(mResourceManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mTextureManager);
	SAFE_DELETE(mEffectManager);
	SAFE_DELETE(mMeshManager);
	SAFE_DELETE(mCamera);
	SAFE_DELETE(mLog);
	// don't need to delete
	mBackGroundNode = nullptr;
}

bool Scene::Initialise(HWND hWnd)
{
	mHwnd = hWnd;
	if (mRenderSystem->Initialise(WINDOW_WIDTH, WINDOW_HEIGHT, mHwnd) == false)
	{
		return false;
	}
	if (mResourceManager->Initialise() == false)
	{
		return false;
	}
	mMaterialManager->Initialise();
	mBackGroundNode = mRootSceneNode->CreateChild("BackGround_Node", Vector3(0, 0, (FAR_PLANE - 10.0f)), Quaternion::IDENTITY, Vector3(1, 1, 1), RenderGroup_BackGroud);
	CreateBackGround();
	return true;
}

void Scene::Update()
{
	mEffectManager->Update();
	RenderOneFrame();
	return;
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

EffectManager* Scene::GetEffectManager() const
{
	return mEffectManager;
}

Camera* Scene::GetCurrentCamera() const
{
	return mCamera;
}

LogImpl* Scene::GetLogImpl() const
{
	return mLog;
}

std::string Scene::GetApplicationPath() const
{
	return mApplicationPath;
}

SceneNode* Scene::GetRootSceneNode() const
{
	return mRootSceneNode;
}

SceneNode* Scene::GetBackGroundNode() const
{
	return mBackGroundNode;
}

void Scene::RenderOneFrame()
{
	mRenderSystem->SetAndClearRenderTarget();
	mRenderSystem->SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	mRenderGroupManager->RenderAllQueue();
	mRenderSystem->Present();
}


void Scene::CreateBackGround()
{
	Mesh* M = mMeshManager->CreateQuad("Back_Ground_Image_0");

	Material* Mat = mMaterialManager->CreateMaterial("Back_Ground_Material_0", SimpleTextureSample);

	std::string BackGroundImageFileName = mResourceManager->GetRandomBackGroundImagePath();

	D3d11Texture* Tex = mTextureManager->LoadTextureFromFile(BackGroundImageFileName, mRenderSystem->GetD3d11Device(), BackGroundImageFileName.c_str(), false);

	Mat->SetTexture(Tex);
	M->SetMaterial(Mat);

	mBackGroundNode->AttachMesh(M);
}