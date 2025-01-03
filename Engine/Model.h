#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "ReleasePtr.h"
#include "Texture.h"

class D3D;

class Model
{
public:

	void Initialize(D3D& d3D);
	void Render(D3D& d3D);

	int GetIndexCount();

	virtual ~Model() = default;

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
	void RenderBuffers(D3D& d3D);

	ReleasePtr<ID3D11Buffer> _vertexBuffer;
	ReleasePtr<ID3D11Buffer> _indexBuffer;
	std::unique_ptr<Texture> _texture;
};