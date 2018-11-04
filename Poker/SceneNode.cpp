/*!
 * \file SceneNode.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#include "SceneNode.h"
#include "Scene.h"

SceneNode::SceneNode(std::string Name)
	: mName(Name)
{
	mPosition = Vector3::ZERO;
	mRotation = Quaternion::IDENTITY;
	mScale = Vector3(1, 1, 1);

	mWorldPosition = Vector3::ZERO;
	mWorldRotation = Quaternion::IDENTITY;
	mWorldScale = Vector3(1, 1, 1);

	mParentNode = nullptr;
}

SceneNode::~SceneNode()
{
	DestroyAllChild();
}

std::string SceneNode::GetName() const
{
	return mName;
}

Vector3 SceneNode::GetPosition() const
{
	return mPosition;
}

Quaternion SceneNode::GetRotation() const
{
	return mRotation;
}

Vector3 SceneNode::GetScale() const
{
	return mScale;
}

Vector3 SceneNode::GetWorldPosition() const
{
	return mWorldPosition;
}

Quaternion SceneNode::GetWorldRotation() const
{
	return mWorldRotation;
}

Vector3 SceneNode::GetWorldScale() const
{
	return mWorldScale;
}

void SceneNode::SetPosition(const Vector3& Pos)
{
	if (Pos != mPosition)
	{
		mPosition = Pos;
		_NotifyModify(mParentNode);
	}
}

void SceneNode::SetRotation(const Quaternion& Rot)
{
	if (Rot != mRotation)
	{
		mRotation = Rot;
		_NotifyModify(mParentNode);
	}
}

void SceneNode::SetScale(const Vector3& Scale)
{
	if (Scale != mScale)
	{
		mScale = Scale;
		_NotifyModify(mParentNode);
	}
}

SceneNode* SceneNode::CreateChild(std::string Name, Vector3 Pos, Quaternion Rot, Vector3 Scale, RenderGroup RG/* = RenderGroup_Normal*/)
{
	SceneNode* Child = new SceneNode(Name);
	Child->SetPosition(Pos);
	Child->SetRotation(Rot);
	Child->SetScale(Scale);
	Child->_NotifyModify(this);
	Child->mRenderGroup = RG;
	mChildArray.push_back(Child);
	Scene::GetCurrentScene()->GetRenderGroupManager()->AddSceneNode(Child);
	return Child;
}

bool SceneNode::RemoveAndDestroyChild(SceneNode* SN)
{
	for (int i = 0; i < mChildArray.size(); i++)
	{
		if (mChildArray[i] == SN)
		{
			SN->DestroyAllChild();
			Scene::GetCurrentScene()->GetRenderGroupManager()->RemoveSceneNode(SN);
			SAFE_DELETE(SN);
			mChildArray.erase(mChildArray.begin() + i);
			return true;
		}
	}
	return false;
}

bool SceneNode::RemoveAndDestroyChild(std::string Name)
{
	for (int i = 0; i < mChildArray.size(); i++)
	{
		if (mChildArray[i]->GetName() == Name)
		{
			mChildArray[i]->DestroyAllChild();
			Scene::GetCurrentScene()->GetRenderGroupManager()->RemoveSceneNode(mChildArray[i]);
			SAFE_DELETE(mChildArray[i]);
			mChildArray.erase(mChildArray.begin() + i);
			return true;
		}
	}
	return false;
}

void SceneNode::DestroyAllChild()
{
	for (int i = 0; i < mChildArray.size(); i++)
	{
		mChildArray[i]->DestroyAllChild();
		Scene::GetCurrentScene()->GetRenderGroupManager()->RemoveSceneNode(mChildArray[i]);
		SAFE_DELETE(mChildArray[i]);
	}
	mChildArray.clear();
}

bool SceneNode::AttachMesh(Mesh* M)
{
	for (int i = 0; i < mAttachMeshArray.size(); i++)
	{
		if (mAttachMeshArray[i] == M)
		{
			return false;
		}
	}
	mAttachMeshArray.push_back(M);
	return false;
}

bool SceneNode::DetachMesh(Mesh* M)
{
	for (int i = 0; i < mAttachMeshArray.size(); i++)
	{
		if (mAttachMeshArray[i] == M)
		{
			mAttachMeshArray.erase(mAttachMeshArray.begin() + i);
			return true;
		}
	}
	return false;
}

void SceneNode::Scale(const Vector3& scale)
{
	mScale = scale;
	_NotifyModify(mParentNode);
}

void SceneNode::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void SceneNode::Translate(float x, float y, float z)
{
	Translate(Vector3(x, y, z));	
}

void SceneNode::Translate(const Vector3& d)
{
	mPosition += d;
	_NotifyModify(mParentNode);
}

void SceneNode::Rotate(const Quaternion& q)
{
	mRotation = mRotation * q;
	_NotifyModify(mParentNode);
}

void SceneNode::Rotate(const Vector3& axis, const Radian& angle)
{
	Quaternion q;
	q.FromAngleAxis(angle, axis);
	Rotate(q);
}

void SceneNode::Roll(const Radian& angle)
{
	Rotate(Vector3::UNIT_Z, angle);
}

void SceneNode::Yaw(const Radian& angle)
{
	Rotate(Vector3::UNIT_Y, angle);
}

void SceneNode::Pitch(const Radian& angle)
{
	Rotate(Vector3::UNIT_X, angle);
}

int SceneNode::GetAttachMeshCount() const
{
	return (int)mAttachMeshArray.size();
}

Mesh* SceneNode::GetAttachMeshByIndex(int Index) const 
{
	return mAttachMeshArray[Index];
}

Mesh* SceneNode::GetAttachMeshByName(std::string Name) const
{
	for (int i = 0; i < mAttachMeshArray.size(); i++)
	{
		if (mAttachMeshArray[i]->GetName() == Name)
		{
			return mAttachMeshArray[i];
		}
	}
	return nullptr;
}

int SceneNode::GetChildCount() const
{
	return (int)mChildArray.size();
}

SceneNode* SceneNode::GetChildByIndex(int Index) const
{
	return mChildArray[Index];
}

SceneNode* SceneNode::GetChildByName(std::string Name) const
{
	for (int i = 0; i < mChildArray.size(); i++)
	{
		if (mChildArray[i]->GetName() == Name)
		{
			return mChildArray[i];
		}
	}
	return nullptr;
}

Matrix4 SceneNode::GetWorldTransform() const
{
	Matrix4 Mat;
	Mat.makeTransform(mWorldPosition, mWorldScale, mWorldRotation);
	// Destination matrix is a column matrix ,must be transpose.
	return Mat.transpose();
}

XMMATRIX SceneNode::GetWorldTransformD3DMath() const
{
	XMVECTOR zero = XMLoadFloat3(&XMFLOAT3(0, 0, 0));
	XMFLOAT3 Pos = XMFLOAT3(mWorldPosition.x, mWorldPosition.y, mWorldPosition.z);
	XMFLOAT4 Rot = XMFLOAT4(mWorldRotation.x, mWorldRotation.y, mWorldRotation.z, mWorldRotation.w);
	XMFLOAT3 Scale = XMFLOAT3(mScale.x, mScale.y, mScale.z);
	XMVECTOR qId = XMLoadFloat4(&XMFLOAT4(0, 0, 0, 1));
	XMVECTOR S = XMLoadFloat3(&Scale);
	XMVECTOR P = XMLoadFloat3(&Pos);
	XMVECTOR Q = XMLoadFloat4(&Rot);

	XMMATRIX mtx = XMMatrixTransformation(zero, qId, S, zero, Q, P);

	return mtx;
}

void SceneNode::_NotifyModify(SceneNode* Parent)
{
	mParentNode = Parent;
	if (mParentNode == nullptr)
	{
		// this is the root node
		mWorldPosition = mPosition;
		mWorldRotation = mRotation;
		mWorldScale = mScale;
	}
	else
	{
		mWorldScale = mParentNode->GetWorldScale() * mScale;
		mWorldRotation = mParentNode->GetWorldRotation() * mRotation;
		mWorldPosition = mParentNode->GetWorldRotation() * mPosition + mParentNode->GetWorldPosition();
	}
	_UpdateAllChild();
}

void SceneNode::_UpdateAllChild()
{
	for (size_t i = 0; i < mChildArray.size(); i++)
	{
		mChildArray[i]->_NotifyModify(this);
	}
}

RenderGroup SceneNode::GetRenderGroup() const
{
	return mRenderGroup;
}