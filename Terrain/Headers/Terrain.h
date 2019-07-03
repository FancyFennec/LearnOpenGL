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
	void updateTerrain();

private:
	PerlinNoise pNoise;

	int getIndex(int i, int j) { return j * width + i; };
	int getMeshIndex(int i, int j) { return (j * (width -1) + i) * 9 * 6; };

	void setMeshColour();
	void setMeshCoordinates();
	void generateHeightMap(float elevation, float frequency, unsigned int octaves, float persistence);
	void setHeightInMesh(int i, int j, float h1, float h2, float h3, float h4);
	void addPointToMesh(int offset, glm::vec3 &v);
};

