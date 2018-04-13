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


class RenderSystemD3D11;

class Scene
{
public:
	Scene();
	~Scene();

	static Scene* GetCurrentScene();
	static void SetCurrentScene(Scene* S);
public:
	RenderSystemD3D11* GetRenderSystem() const;

public:
	void OnKeyDown(unsigned char Key);
	void OnKeyUp(unsigned char Key);
	void OnLButtonDown(int x, int y, unsigned int wParam);
	void OnLButtonUp(int x, int y, unsigned int wParam);
	void OnRButtonDown(int x, int y, unsigned int wParam);
	void OnRButtonUp(int x, int y, unsigned int wParam);
	void OnMouseMove(int x, int y, unsigned int wParam);
	void OnLButtonDbclk(int x, int y, unsigned int wParam);
	void OnRButtonDbclk(int x, int y, unsigned int wParam);

protected:
	static Scene* CurrentScene;

private:
	RenderSystemD3D11* mRS;

};