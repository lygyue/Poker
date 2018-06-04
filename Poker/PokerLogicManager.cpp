/***********************************************
 * File: PokerLogicManager.cpp
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#include "PokerLogicManager.h"
#include "Math/Vector3.h"
#include "Scene.h"

PokerLogicManager::PokerLogicManager()
{
	mCardManager = new CardManager;
	mCardDealer = new RamdomDealer;
	Scene* S = new Scene;
	Scene::SetCurrentScene(S);
	BuildPokerName();
	InitialiseAllResource();
}

PokerLogicManager::~PokerLogicManager()
{
	Scene* S = Scene::GetCurrentScene();
	SAFE_DELETE(S);
	Scene::SetCurrentScene(nullptr);
	SAFE_DELETE(mCardManager);
	SAFE_DELETE(mCardDealer);
}

void PokerLogicManager::BuildPokerName()
{
	char* szPoker[] = { "Diamond_", "CLub_", "Heart_", "Spade_" };
	for (int i = 0; i < Primary; i++)
	{
		for (int j = 0; j < Poker_Black_Joker; j++)
		{
			mPokerResourceName[i][j] = szPoker[i];
			char Buff[4] = { 0 };
			sprintf_s(Buff, 4, "%d", j);
			mPokerResourceName[i][j] += Buff;
		}
	}
	mBlackJokerName = "Black_Joker";
	mRedJokerName = "Red_Joker";
}

void PokerLogicManager::InitialiseAllResource()
{
	Vector3 vertex[4];
	float half_width = POKER_WIDTH / 2.0f;
	float half_height = POKER_HEIGHT / 2.0f;
	// left top
	vertex[0] = Vector3(-half_width, half_height, 0);
	// right top
	vertex[1] = Vector3(half_width, half_height, 0);
	// right bottom
	vertex[2] = Vector3(half_width, -half_height, 0);
	// left bottom
	vertex[3] = Vector3(-half_width, -half_height, 0);
	TextureManager* TextureMgr = Scene::GetCurrentScene()->GetTextureManager();
	MeshManager* MeshMgr = Scene::GetCurrentScene()->GetMeshManager();
	MaterialManager* MaterialMgr = Scene::GetCurrentScene()->GetMaterialManager();
	ResourceManager* ResourceMgr = Scene::GetCurrentScene()->GetResourceManager();
	RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
	for (int i = 0; i < Primary; i++)
	{
		for (int j = 0; j < Poker_Black_Joker; j++)
		{
			// load textures
			std::string Path = ResourceMgr->GetPokerFullPath(PokerType(j), PokerClassify(i));
			D3d11Texture* Tex = TextureMgr->LoadTextureFromFile(mPokerResourceName[i][j], RS->GetD3d11Device(), Path.c_str(), false);
			// create materials
			Material* Mat = MaterialMgr->CreateMaterial(mPokerResourceName[i][j], SimpleTextureSample);
			Mat->SetTexture(Tex);
			// create meshes
			Mesh* M = MeshMgr->CreateQuad(mPokerResourceName[i][j], vertex);
			M->SetMaterial(Mat);

			// create all cards;
			Card* C = mCardManager->CreateCard(mPokerResourceName[i][j], PokerType(j), PokerClassify(i));
			// four pokers
			mCardDealer->PushCard(C);
			mCardDealer->PushCard(C);
			mCardDealer->PushCard(C);
			mCardDealer->PushCard(C);
		}
	}

	// create red and black joker
	std::string Path = ResourceMgr->GetPokerFullPath(Poker_Black_Joker, Primary);
	D3d11Texture* Tex = TextureMgr->LoadTextureFromFile(mBlackJokerName, RS->GetD3d11Device(), Path.c_str(), false);
	// create materials
	Material* Mat = MaterialMgr->CreateMaterial(mBlackJokerName, SimpleTextureSample);
	Mat->SetTexture(Tex);
	// create meshes
	Mesh* M = MeshMgr->CreateQuad(mBlackJokerName, vertex);
	M->SetMaterial(Mat);
	Card* C = mCardManager->CreateCard(mBlackJokerName, Poker_Black_Joker, Primary);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);

	Path = ResourceMgr->GetPokerFullPath(Poker_Red_Joker, Primary);
	Tex = TextureMgr->LoadTextureFromFile(mRedJokerName, RS->GetD3d11Device(), Path.c_str(), false);
	// create materials
	Mat = MaterialMgr->CreateMaterial(mRedJokerName, SimpleTextureSample);
	Mat->SetTexture(Tex);
	// create meshes
	M = MeshMgr->CreateQuad(mRedJokerName, vertex);
	M->SetMaterial(Mat);
	C = mCardManager->CreateCard(mRedJokerName, Poker_Red_Joker, Primary);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);
	mCardDealer->PushCard(C);
}

std::string PokerLogicManager::GetPokerName(PokerType PT, PokerClassify PC) const
{
	if (PT == Poker_Black_Joker)
	{
		return mBlackJokerName;
	}
	else if (PT == Poker_Red_Joker)
	{
		return mRedJokerName;
	}
	else
	{
		return mPokerResourceName[PC][PT];
	}
}


void PokerLogicManager::OnKeyDown(unsigned char Key)
{

}

void PokerLogicManager::OnKeyUp(unsigned char Key)
{

}

void PokerLogicManager::OnLButtonDown(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnLButtonUp(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnRButtonDown(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnRButtonUp(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnLButtonDbclk(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnRButtonDbclk(int x, int y, unsigned int wParam)
{

}

void PokerLogicManager::OnMouseMove(int x, int y, unsigned int wParam)
{

}