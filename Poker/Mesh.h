/*!
 * \file Mesh.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include "Common.h"
#include "Material.h"
#include <map>
class MeshManager;
class Mesh
{
	friend class MeshManager;
public:
	std::string GetName() const;
	Material* GetMaterial() const;
	void SetMaterial(Material* Mat);

protected:
	Mesh(std::string Name);
	~Mesh();

	bool Initialise(void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount);
private:
	std::string mName;
	Material* mMaterial;
	DataBuffer* mVertexBuffer;
	DataBuffer* mIndexBuffer;
};

class MeshManager
{
	friend class Scene;
public:
	Mesh* CreateMesh(std::string Name, void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount);

	bool DestroyMesh(Mesh*);
	bool DestroyMesh(std::string Name);
	void DestroyAllMesh();
protected:
	MeshManager();
	~MeshManager();
private:
	std::map<std::string, Mesh*> mMeshArray;
};
