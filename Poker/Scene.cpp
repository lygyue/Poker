/*!
 * \file Scene.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "Scene.h"
#include "RenderSystem.h"

Scene* Scene::CurrentScene = nullptr;
Scene::Scene()
{
	mRS = nullptr;
}

Scene::~Scene()
{

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
	return mRS;
}


void Scene::OnKeyDown(unsigned char Key)
{

}

void Scene::OnKeyUp(unsigned char Key)
{

}

void Scene::OnLButtonDown(int x, int y, unsigned int wParam)
{

}

void Scene::OnLButtonUp(int x, int y, unsigned int wParam)
{

}

void Scene::OnRButtonDown(int x, int y, unsigned int wParam)
{

}

void Scene::OnRButtonUp(int x, int y, unsigned int wParam)
{

}

void Scene::OnLButtonDbclk(int x, int y, unsigned int wParam)
{

}

void Scene::OnRButtonDbclk(int x, int y, unsigned int wParam)
{

}

void Scene::OnMouseMove(int x, int y, unsigned int wParam)
{

}