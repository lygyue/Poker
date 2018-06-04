/***********************************************
 * File: RenderGroupManager.h
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#pragma once
#include "Common.h"
#include <vector>

enum RenderGroup
{
	RenderGroup_BackGroud,
	RenderGroup_BeforeNormal,
	RenderGroup_Normal,
	RenderGroup_AfterNormal,
	RenderGroup_Final,
};

class SceneNode;
class Scene;
typedef std::vector<SceneNode*> RenderQueue;
class RenderGroupManager
{
	friend class SceneNode;
	friend class Scene;
public:


protected:
	RenderGroupManager();
	~RenderGroupManager();

	void AddSceneNode(SceneNode* SN);
	void RemoveSceneNode(SceneNode* SN);

	void RenderAllQueue() const;
private:
	std::vector<RenderQueue> mRenderGroups;
};
