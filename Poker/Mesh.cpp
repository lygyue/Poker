/*!
 * \file Mesh.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */

#include "Mesh.h"

Mesh::Mesh(std::string Name)
{
	mName = Name;
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
}

Mesh::~Mesh()
{
	SAFE_DELETE(mVertexBuffer);
	SAFE_DELETE(mIndexBuffer);
}

std::string Mesh::GetName() const
{
	return mName;
}

Material* Mesh::GetMaterial() const
{
	return mMaterial;
}

void Mesh::SetMaterial(Material* Mat)
{
	mMaterial = Mat;
}

bool Mesh::Initialise(void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount)
{
	VertexBuffer = new DataBuffer(Device, D3D11_BIND_VERTEX_BUFFER, &t->Vertices[0], t->numVertices * sizeof(Vector3));
	IndexBuffer = new DataBuffer(Device, D3D11_BIND_INDEX_BUFFER, &t->Indices[0], t->numIndices * sizeof(short));
	return true;
}