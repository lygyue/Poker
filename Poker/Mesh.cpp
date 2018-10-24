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
#include "Math/Vector2.h"

Mesh::Mesh(std::string Name)
{
	mName = Name;
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mIndexFormat = DXGI_FORMAT_R16_UINT;
	mVertexElementSize = 12;
	mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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


bool Mesh::Initialise(void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount, D3D11_PRIMITIVE_TOPOLOGY Primitive/* = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
{
	ID3D11Device* Device = Scene::GetCurrentScene()->GetRenderSystem()->GetD3d11Device();
	int IndexBufferLength = IndexCount > 0xFFFF ? IndexCount * sizeof(int) : IndexCount * sizeof(short);
	if (IndexCount > 0xFFFF)
	{
		mIndexFormat = DXGI_FORMAT_R32_UINT;
	}
	else
	{
		mIndexFormat = DXGI_FORMAT_R16_UINT;
	}
	mPrimitiveType = Primitive;
	mVertexElementSize = VertexElementSize;
	mVertexCount = VertexCount;
	mIndexCount = IndexCount;
	mVertexBuffer = new DataBuffer(Device, D3D11_BIND_VERTEX_BUFFER, VertexBuffer, VertexCount * VertexElementSize);
	mIndexBuffer = new DataBuffer(Device, D3D11_BIND_INDEX_BUFFER, IndexBuffer, IndexBufferLength);
	return true;
}

void Mesh::RenderMesh(Matrix4& WorldTransform) const
{
	XMMATRIX modelMat = XMMATRIX(WorldTransform[0][0], WorldTransform[0][1], WorldTransform[0][2], WorldTransform[0][3],
		WorldTransform[1][0], WorldTransform[1][1], WorldTransform[1][2], WorldTransform[1][3],
		WorldTransform[2][0], WorldTransform[2][1], WorldTransform[2][2], WorldTransform[2][3],
		WorldTransform[3][0], WorldTransform[3][1], WorldTransform[3][2], WorldTransform[3][3]);

	RenderMesh(modelMat);
}

void Mesh::RenderMesh(XMMATRIX& WorldTransform) const
{
	char UniformBuffer[MAX_CONST_BUFFER];
	memset(UniformBuffer, 0, sizeof(UniformBuffer));
	XMMATRIX ViewProjection = Scene::GetCurrentScene()->GetCurrentCamera()->GetProjectViewMatrix();
	XMMATRIX FinalMatrix = XMMatrixMultiply(WorldTransform, ViewProjection);
	memcpy(UniformBuffer + 0, &FinalMatrix, 64);
	if (mMaterial->GetConstBufferLen() > 0 && (64 + mMaterial->GetConstBufferLen()) < MAX_CONST_BUFFER)
	{
		char* TempPtr = mMaterial->GetConstBufferPointer();
		int Len = mMaterial->GetConstBufferLen();
		memcpy(UniformBuffer + 64, TempPtr, Len);
	}
	RenderSystemD3D11* RS = Scene::GetCurrentScene()->GetRenderSystem();
	ID3D11DeviceContext* Context = RS->GetD3d11Context();
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE map;
	hr = Context->Map(RS->GetUniformBuffer()->D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, UniformBuffer, MAX_CONST_BUFFER);
	Context->Unmap(RS->GetUniformBuffer()->D3DBuffer, 0);

	Context->IASetInputLayout(mMaterial->mShader->GetD3d11InputLayout());
	Context->IASetIndexBuffer(mIndexBuffer->D3DBuffer, mIndexFormat, 0);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &mVertexBuffer->D3DBuffer, &mVertexElementSize, &offset);
	Context->IASetPrimitiveTopology(mPrimitiveType);
	Context->VSSetShader(mMaterial->mShader->GetD3d11VertexShader(), NULL, 0);
	Context->PSSetShader(mMaterial->mShader->GetD3d11PixelShader(), NULL, 0);
	Context->PSSetSamplers(0, 1, &mMaterial->mSamplerState);
	int TextureCount = mMaterial->GetTextureCount();
	for (int i = 0; i < TextureCount; i++)
	{
		Context->PSSetShaderResources(i, 1, mMaterial->GetTexture(i)->GetShaderResourceViewPtr());
	}
	Context->RSSetState(mMaterial->mRasterizer);
	Context->OMSetDepthStencilState(mMaterial->mDepthState, 0);
	Context->OMSetBlendState(mMaterial->mBlendState, NULL, 0xffffffff);

	Context->DrawIndexed(mIndexCount, 0, 0);
}

MeshManager::MeshManager()
{

}

MeshManager::~MeshManager()
{
	DestroyAllMesh();
}

Mesh* MeshManager::CreateMesh(std::string Name, void* VertexBuffer, int VertexElementSize, int VertexCount, void* IndexBuffer, int IndexCount)
{
	if (mMeshArray.find(NULL) != mMeshArray.end())
	{
		return mMeshArray[Name];
	}
	Mesh* M = new Mesh(Name);
	bool ret = M->Initialise(VertexBuffer, VertexElementSize, VertexCount, IndexBuffer, IndexCount);
	if (ret)
	{
		mMeshArray[Name] = M;
		return M;
	}
	SAFE_DELETE(M);
	return nullptr;
}

Mesh* MeshManager::CreateQuad(std::string Name, Vector3* Vertex/* = nullptr*/, Vector2* TexCoord/* = nullptr*/)
{
	if (mMeshArray.find(Name) != mMeshArray.end())
	{
		return mMeshArray[Name];
	}
	struct QuadStruct 
	{
		Vector3 Position;
		Vector2 TexCoord;
	};
	QuadStruct QS[4];
	if (Vertex)
	{
		QS[0].Position = Vertex[0];
		QS[1].Position = Vertex[1];
		QS[2].Position = Vertex[2];
		QS[3].Position = Vertex[3];
	}
	else
	{
		QS[0].Position = Vector3(-1, 1, 0);
		QS[1].Position = Vector3(1, 1, 0);
		QS[2].Position = Vector3(1, -1, 0);
		QS[3].Position = Vector3(-1, -1, 0);
	}
	if (TexCoord)
	{
		QS[0].TexCoord = TexCoord[0];
		QS[1].TexCoord = TexCoord[1];
		QS[2].TexCoord = TexCoord[2];
		QS[3].TexCoord = TexCoord[3];
	}
	else
	{
		QS[0].TexCoord = Vector2(0, 0);
		QS[1].TexCoord = Vector2(1, 0);
		QS[2].TexCoord = Vector2(1, 1);
		QS[3].TexCoord = Vector2(0, 1);
	}

	// assume as 0, 1, 2, 3
	short Indexbuffer[6] = { 0, 1, 3, 1, 2, 3 };
	Mesh* M = new Mesh(Name);
	M->Initialise(QS, sizeof(QuadStruct), 4, Indexbuffer, 6);
	mMeshArray[Name] = M;
	return M;
}

Mesh* MeshManager::CreateLine(std::string Name, Vector3* Vertex)
{
	if (mMeshArray.find(NULL) != mMeshArray.end())
	{
		return mMeshArray[Name];
	}
	// assume as 0, 1, 2, 3
	short Indexbuffer[2] = { 0, 1};
	Mesh* M = new Mesh(Name);
	M->Initialise(Vertex, sizeof(Vector3), 2, Indexbuffer, 2, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	mMeshArray[Name] = M;
	return M;
}

Mesh* MeshManager::CreateSphere(std::string Name, int Col, int Row, float Radius)
{
	return nullptr;
}

Mesh* MeshManager::CreateBox(std::string Name, const Vector3& V)
{
	struct VertexElement
	{
		Vector3 Pos;
		Vector2 UV;
	};

	return nullptr;
}

Mesh* MeshManager::GetMeshByName(std::string Name)
{
	if (mMeshArray.find(Name) == mMeshArray.end())
	{
		return nullptr;
	}
	return mMeshArray[Name];
}

bool MeshManager::DestroyMesh(std::string Name)
{
	if (mMeshArray.find(Name) == mMeshArray.end())
	{
		return false;
	}
	Mesh* M = mMeshArray[Name];
	mMeshArray.erase(Name);
	SAFE_DELETE(M);
	return true;
}

bool MeshManager::DestroyMesh(Mesh* M)
{
	std::string Name = M->GetName();
	return DestroyMesh(Name);
}

void MeshManager::DestroyAllMesh()
{
	std::map<std::string, Mesh*>::iterator it;
	for (it = mMeshArray.begin(); it != mMeshArray.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	mMeshArray.clear();
}