/*!
 * \file Mesh.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */

#include "Mesh.h"
#include "Scene.h"
#include "RenderSystem.h"

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
	ID3D11Device* Device = Scene::GetCurrentScene()->GetRenderSystem()->GetD3d11Device();
	int IndexBufferLength = IndexCount > 0xFFFF ? IndexCount * sizeof(int) : IndexCount * sizeof(short);
	VertexBuffer = new DataBuffer(Device, D3D11_BIND_VERTEX_BUFFER, VertexBuffer, VertexCount * VertexElementSize);
	IndexBuffer = new DataBuffer(Device, D3D11_BIND_INDEX_BUFFER, IndexBuffer, IndexBufferLength);
	return true;
}