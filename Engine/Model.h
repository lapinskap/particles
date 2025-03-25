#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "ReleasePtr.h"
#include "Texture.h"

class D3D;

class Model
{
public:

	virtual ~Model() = default;

	void Initialize(D3D& d3D);
	void ApplyBuffers(D3D& d3D);

	size_t GetVertexSize() const;
	uint GetIndexCount() const;
	ID3D11Buffer* GetVertexBuffer();

protected:

	struct Vertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normal;
	};


	virtual std::vector<Vertex> CreateVertices() const;
	virtual std::vector<uint> CreateIndices() const;
	int _vertexCount = 0;
	int _indexCount = 0;

private:

	void InitializeBuffers(D3D& d3D);

	ReleasePtr<ID3D11Buffer> _vertexBuffer;
	ReleasePtr<ID3D11Buffer> _indexBuffer;
};