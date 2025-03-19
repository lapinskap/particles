#include "SphereModel.h"
#include <iostream>
#include <cmath>

const int NUM_SEGMENTS = 10;

std::vector<InstancedModel::Vertex> SphereModel::CreateVertices() const
{
	std::vector<Vertex> vertices;

	// (polar angle) latitude
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		//  (azimuthal angle) longitude
		for (int j = 0; j < NUM_SEGMENTS; j++)
		{

			float azimuthalAngle = DirectX::XM_2PI * j / NUM_SEGMENTS;
			float polarAngle = -DirectX::XM_PIDIV2 + DirectX::XM_PI * ((float)i / NUM_SEGMENTS);
			polarAngle += DirectX::XM_PI * 0.5f / NUM_SEGMENTS;

			float x = cos(azimuthalAngle) * cos(polarAngle);

			float y = sin(polarAngle);

			float z = sin(azimuthalAngle) * cos(polarAngle);

			//std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

			DirectX::XMFLOAT4 position = { x, y, z, 1.0f };
			DirectX::XMFLOAT4 color = { modelColor.x, modelColor.y,modelColor.z, fabs(azimuthalAngle - DirectX::XM_PI)};
			DirectX::XMFLOAT4 normal = { x, y, z, 0.0f };

			vertices.push_back({ position, color, normal });
		}
	}

	// Add the poles

	// South pole
	{
		DirectX::XMFLOAT4 position = { 0.0f, -1.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 color = { modelColor.x, modelColor.y,modelColor.z, 0.0f };
		DirectX::XMFLOAT4 normal = { 0.0f, -1.0f, 0.0f, 0.0f };

		vertices.push_back({ position, color, normal });
	}

	// North pole
	{
		DirectX::XMFLOAT4 position = { 0.0f, 1.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 color = { modelColor.x, modelColor.y,modelColor.z, 0.0f };
		DirectX::XMFLOAT4 normal = { 0.0f, 1.0f, 0.0f, 0.0f };

		vertices.push_back({ position, color, normal });
	}

	std::cout << "SphereModel CreateVertices, size: " << vertices.size() << std::endl;

	return vertices;
}

std::vector<uint> SphereModel::CreateIndices() const
{
	std::vector<uint> indices;

	// (polar angle) latitude in degrees
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		//  (azimuthal angle) longitude in degrees
		for (int j = 0; j < NUM_SEGMENTS; j++)
		{
			indices.push_back(i * NUM_SEGMENTS + j); // 0, 1
			indices.push_back(i * NUM_SEGMENTS + (j + 1) % NUM_SEGMENTS); // 1, 2
			indices.push_back(((i + 1) % NUM_SEGMENTS) * NUM_SEGMENTS + (j + 1) % NUM_SEGMENTS);// 11, 12

			indices.push_back(((i + 1) % NUM_SEGMENTS) * NUM_SEGMENTS + (j + 1) % NUM_SEGMENTS); // 11, 12
			indices.push_back(((i + 1) % NUM_SEGMENTS) * NUM_SEGMENTS + j); // 10, 11
			indices.push_back(i * NUM_SEGMENTS + j); // 0, 1
		}
	}

	constexpr int southPoleIndex = NUM_SEGMENTS * NUM_SEGMENTS;
	constexpr int northPoleIndex = southPoleIndex + 1;

	// Poles
	// South pole
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		indices.push_back(i);
		indices.push_back((i + 1) % NUM_SEGMENTS);
		indices.push_back(southPoleIndex);
	}
	// North pole
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		indices.push_back((NUM_SEGMENTS - 1) * NUM_SEGMENTS + i);
		indices.push_back((NUM_SEGMENTS - 1) * NUM_SEGMENTS + (i + 1) % NUM_SEGMENTS);
		indices.push_back(northPoleIndex);
	}

	std::cout << "SphereModel index count: " << indices.size() << std::endl;

	return indices;
}
