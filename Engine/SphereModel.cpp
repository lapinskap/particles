#include "SphereModel.h"
#include <iostream>

const int NUM_SEGMENTS = 10;

std::vector<Model::Vertex> SphereModel::CreateVertices() const
{
	std::vector<Vertex> vertices;

	// (polar angle) latitude in degrees
	for (int i = 0; i < NUM_SEGMENTS; i++)
	{
		//  (azimuthal angle) longitude in degrees
		for (int j = 0; j < NUM_SEGMENTS; j++)
		{
			float azimuthalAngle = DirectX::XM_2PI * j / NUM_SEGMENTS;
			float polarAngle = (DirectX::XM_PI / 2) * ((float)i / NUM_SEGMENTS);

			float x = cos(azimuthalAngle) * cos(polarAngle);
			float y = sin(polarAngle);
			float z = sin(azimuthalAngle) * cos(polarAngle);

			std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

			DirectX::XMFLOAT4 position = { x, y, z, 1.0f };
			DirectX::XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
			DirectX::XMFLOAT4 normal = { x, y, z, 0.0f };

			vertices.push_back({ position, color, normal });
		}
	}

	return vertices;
}

std::vector<uint> SphereModel::CreateIndices() const
{
	//return std::vector<uint> {0, 1, 2, 1, 3, 2};
	std::vector<uint> indices;

	// (polar angle) latitude in degrees
	for (int i = 0; i < NUM_SEGMENTS - 1; i++)
	{
		//  (azimuthal angle) longitude in degrees
		for (int j = 0; j < NUM_SEGMENTS - 1; j++)
		{
			indices.push_back(i * NUM_SEGMENTS + j);
			indices.push_back(i * NUM_SEGMENTS + j + 1);
			indices.push_back((i + 1) * NUM_SEGMENTS + j + 1);

			indices.push_back((i + 1) * NUM_SEGMENTS + j + 1);
			indices.push_back((i + 1) * NUM_SEGMENTS + j);
			indices.push_back(i * NUM_SEGMENTS + j);
		}
	}

	return indices;
}