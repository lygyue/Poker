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
#include "Math/Matrix4.h"
#include "Mesh.h"
#include "RenderGroupManager.h"
#include <vector>


class SceneNode
{
	friend class Scene;
	friend class RenderGroupManager;
public:
	std::string GetName() const;
	Vector3 GetPosition() const;
	Quaternion GetRotation() const;
	Vector3 GetScale() const;

	Vector3 GetWorldPosition() const;
	Quaternion GetWorldRotation() const;
	Vector3 GetWorldScale() const;

	void SetPosition(const Vector3& Pos);
	void SetRotation(const Quaternion& Rot);
	void SetScale(const Vector3& Scale);

	SceneNode* CreateChild(std::string Name, Vector3 Pos, Quaternion Rot, Vector3 Scale, RenderGroup RG = RenderGroup_Normal);

	bool RemoveAndDestroyChild(std::string Name);
	bool RemoveAndDestroyChild(SceneNode* SN);
	void DestroyAllChild();

	bool AttachMesh(Mesh* M);
	bool DetachMesh(Mesh* M);

	void SetVisible(bool Visible);
	bool GetVisible() const;

	void Scale(const Vector3& scale);
	void Scale(float x, float y, float z);
	void Translate(const Vector3& d);
	void Translate(float x, float y, float z);
	void Roll(const Radian& angle);
	void Pitch(const Radian& angle);
	void Yaw(const Radian& angle);
	void Rotate(const Vector3& axis, const Radian& angle);
	void Rotate(const Quaternion& q);

	int GetAttachMeshCount() const;
	Mesh* GetAttachMeshByIndex(int Index) const;
	Mesh* GetAttachMeshByName(std::string Name) const;

	int GetChildCount() const;
	SceneNode* GetChildByIndex(int Index) const;
	SceneNode* GetChildByName(std::string Name) const;

	Matrix4 GetWorldTransform() const;
	XMMATRIX GetWorldTransformD3DMath() const;
protected:
	SceneNode(std::string Name);
	~SceneNode();

	void _NotifyModify(SceneNode* Parent);
	void _UpdateAllChild();
	
	RenderGroup GetRenderGroup() const;
private:
	SceneNode* mParentNode;
	std::string mName;

	Vector3 mPosition;
	Quaternion mRotation;
	Vector3 mScale;

	Vector3 mWorldPosition;
	Quaternion mWorldRotation;
	Vector3 mWorldScale;

	RenderGroup mRenderGroup;

	bool mVisible;

	std::vector<SceneNode*> mChildArray;
	std::vector<Mesh*> mAttachMeshArray;
};
