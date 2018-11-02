/*!
 * \file Mesh.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Common.h"
#include "Material.h"
#include <map>
class MeshManager;
class Mesh
{
	friend class MeshManager;
	friend class RenderGroupManager;
public:
	std::string GetName() const;
	Material* GetMaterial() const;
	void SetMaterial(Material* Mat);

	void SetVisible(bool Visible);
	bool GetVisible() const;
protected:
	Mesh(std::string Name);
	~Mesh();

	bool Initialise(void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount, D3D11_PRIMITIVE_TOPOLOGY Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void RenderMesh(Matrix4& WorldTransform) const;
	void RenderMesh(XMMATRIX& WorldTransform) const;
private:
	std::string mName;
	Material* mMaterial;
	DataBuffer* mVertexBuffer;
	DataBuffer* mIndexBuffer;
	UINT mVertexCount;
	UINT mVertexElementSize;
	UINT mIndexCount;
	D3D11_PRIMITIVE_TOPOLOGY mPrimitiveType;
	DXGI_FORMAT mIndexFormat;
	bool mVisible;
};

class MeshManager
{
	friend class Scene;
public:
	Mesh* CreateMesh(std::string Name, void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount);
	Mesh* CreateQuad(std::string Name, Vector3* Vertex = nullptr, Vector2* TexCoord = nullptr);
	Mesh* CreateLine(std::string Name, Vector3* Vertex);
	Mesh* CreateSphere(std::string Name, int Col, int Row, float Radius);
	Mesh* CreateBox(std::string Name, const Vector3& V);

	Mesh* GetMeshByName(std::string Name);

	bool DestroyMesh(Mesh* M);
	bool DestroyMesh(std::string Name);
protected:
	MeshManager();
	~MeshManager();

	void DestroyAllMesh();
private:
	std::map<std::string, Mesh*> mMeshArray;
};
