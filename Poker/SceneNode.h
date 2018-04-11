/*!
 * \file SceneNode.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Common.h"
#include "Math/SimpleMath.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

class SceneNode
{
public:
	SceneNode();
	~SceneNode();

private:
	Vector3 mPosition;
	Quaternion mRotation;
};
