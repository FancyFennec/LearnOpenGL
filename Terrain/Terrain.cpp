#include "Headers/Terrain.h"

Terrain::Terrain(int width, int height) :
	width(width),
	height(height),
	pNoise(4),
	terrainColour(0.3f, 0.15f, 0.05f),
	heightMap(width * height),
	vertices((width - 1) * (height - 1) * 9 * 6)
{
	setMeshColour();
	setMeshCoordinates();
}

Terrain::~Terrain()
{
}

void Terrain::setMeshColour()
{
	for (size_t offset = 0; offset < vertices.size(); offset += 9) {
		addPointToMesh(offset + 6, terrainColour);
	}
}

void Terrain::setMeshCoordinates()
{
	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {

			glm::vec3 
				v1(i, 0, j), 
				v2(i + 1, 0, j), 
				v3(i + 1, 0, j + 1), 
				v4(i, 0, j + 1);

			addPointToMesh(getMeshIndex(i, j) + 9 * 0, v1);
			addPointToMesh(getMeshIndex(i, j) + 9 * 1, v2);
			addPointToMesh(getMeshIndex(i, j) + 9 * 2, v3);

			addPointToMesh(getMeshIndex(i, j) + 9 * 3, v1);
			addPointToMesh(getMeshIndex(i, j) + 9 * 4, v3);
			addPointToMesh(getMeshIndex(i, j) + 9 * 5, v4);
		}
	}
}

void Terrain::generateHeightMap(float elevation, float frequency, unsigned int octaves, float persistence)
{
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			heightMap[getIndex(i, j)] =
				elevation * (float) pNoise.octavePerlin(
					frequency * (double)i / ((double)width),
					frequency * (double)j / ((double)height),
					0,
					octaves,
					persistence);
		}
	}
}

void Terrain::generateTerrain(float elevation, float frequency, unsigned int octaves, float persistence)
{
	generateHeightMap(elevation, frequency, octaves, persistence);

	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {

			float h1, h2, h3, h4;

			h1 = heightMap[getIndex(i, j)];
			h2 = heightMap[getIndex(i + 1, j)];
			h3 = heightMap[getIndex(i + 1, j + 1)];
			h4 = heightMap[getIndex(i, j + 1)];

			setHeightInMesh(i, j, h1, h2, h3, h4);
		}
	}
}

void Terrain::addPointToMesh(int offset, glm::vec3 &v)
{
	vertices[offset] = v.x;
	vertices[offset + 1] = v.y;
	vertices[offset + 2] = v.z;
}

void  Terrain::setHeightInMesh(int i, int j, float h1, float h2, float h3, float h4)
{
	glm::vec3 n1 = glm::normalize(glm::vec3(h1 - h2, 1, h2 - h3)); //Hard coded  vector product magic
	glm::vec3 n2 = glm::normalize(glm::vec3(h4 - h3, 1, h1 - h4));
	
	vertices[getMeshIndex(i, j) + 9 * 0 + 1] = h1;
	vertices[getMeshIndex(i, j) + 9 * 1 + 1] = h2;
	vertices[getMeshIndex(i, j) + 9 * 2 + 1] = h3;

	vertices[getMeshIndex(i, j) + 9 * 3 + 1] = h1;
	vertices[getMeshIndex(i, j) + 9 * 4 + 1] = h3;
	vertices[getMeshIndex(i, j) + 9 * 5 + 1] = h4;

	addPointToMesh(getMeshIndex(i, j) + 9 * 0 + 3, n1);
	addPointToMesh(getMeshIndex(i, j) + 9 * 1 + 3, n1);
	addPointToMesh(getMeshIndex(i, j) + 9 * 2 + 3, n1);

	addPointToMesh(getMeshIndex(i, j) + 9 * 3 + 3, n2);
	addPointToMesh(getMeshIndex(i, j) + 9 * 4 + 3, n2);
	addPointToMesh(getMeshIndex(i, j) + 9 * 5 + 3, n2);
}
