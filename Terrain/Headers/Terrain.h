#pragma once

#include "Headers/pNoise.h"
#include <glm/glm.hpp>

class Terrain
{
public:
	Terrain(int width, int height);
	~Terrain();

	int width, height;
	glm::vec3 terrainColour;
	
	std::vector<float> heightMap;
	std::vector<float> vertices;

	void generateTerrain(float elevation, float frequency, unsigned int octaves, float persistence);

private:
	PerlinNoise pNoise;

	int getIndex(int i, int j) { return j * width + i; };

	void generateHeightMap(float &elevation, float &frequency, unsigned int &octaves, float &persistence);
	void addTriangleToMesh(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	void addPointToMesh(glm::vec3 v);
};

