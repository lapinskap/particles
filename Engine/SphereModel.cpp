#include "SphereModel.h"
#include <iostream>
#include <cmath>

const int NUM_SEGMENTS = 20;

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
			float polarAngle = -DirectX::XM_PIDIV2 + DirectX::XM_PI * ((float)i / NUM_SEGMENTS);

			float x = cos(azimuthalAngle) * cos(polarAngle);

			float y = sin(polarAngle);

			float z = sin(azimuthalAngle) * cos(polarAngle);

			//std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

			DirectX::XMFLOAT4 position = { x, y, z, 1.0f };
			DirectX::XMFLOAT4 color = { 0.2f, 0.5f, 0.2f, fabs(azimuthalAngle - DirectX::XM_PI) };
			DirectX::XMFLOAT4 normal = { x, y, z, 0.0f };

			vertices.push_back({ position, color, normal });
		}
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

	std::cout << "SphereModel index count: " << indices.size() << std::endl;

	return indices;
}

//std::vector<Model::Vertex> SphereModel::CreateVertices() const
//{
//    std::vector<Vertex> vertices;
//
//    for (int i = 0; i <= NUM_SEGMENTS; ++i)
//    {
//        float theta = i * DirectX::XM_PI / NUM_SEGMENTS; // polar angle
//
//        for (int j = 0; j <= NUM_SEGMENTS; ++j)
//        {
//            float phi = j * 2 * DirectX::XM_PI / NUM_SEGMENTS; // azimuthal angle
//
//            float x = sin(theta) * cos(phi);
//            float y = cos(theta);
//            float z = sin(theta) * sin(phi);
//
//           std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
//
//            DirectX::XMFLOAT4 position = { x, y, z, 1.0f };
//            DirectX::XMFLOAT4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
//            DirectX::XMFLOAT4 normal = { x, y, z, 0.0f };
//
//            vertices.push_back({ position, color, normal });
//        }
//    }
//
//    std::cout << "SphereModel CreateVertices, size: " << vertices.size() << std::endl;
//
//    return vertices;
//}
//
//std::vector<uint> SphereModel::CreateIndices() const
//{
//    std::vector<uint> indices;
//
//    for (int i = 0; i < NUM_SEGMENTS; ++i)
//    {
//        for (int j = 0; j < NUM_SEGMENTS; ++j)
//        {
//            int first = i * (NUM_SEGMENTS + 1) + j;
//            int second = first + NUM_SEGMENTS + 1;
//
//            indices.push_back(first);
//            indices.push_back(second);
//            indices.push_back(first + 1);
//
//            indices.push_back(second);
//            indices.push_back(second + 1);
//            indices.push_back(first + 1);
//        }
//    }
//
//    std::cout << "SphereModel index count: " << indices.size() << std::endl;
//
//    return indices;
//};
