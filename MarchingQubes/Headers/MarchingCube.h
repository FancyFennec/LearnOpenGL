#pragma once

#include "Tables.h"

class MarchingCube
{
public:
	MarchingCube();
	~MarchingCube();

	
	std::vector<float> generateMesh(float* booleanField, int m, int n, int k);

	std::vector<std::vector<float>> lookupMesh;

private:
	void generateMeshTable();
	void generateTriangle(std::vector<float> &currentMesh, int* currentInt, int k);
	void generateVertex(std::vector<float> & currentMesh, glm::vec3 &v, glm::vec3 &n, glm::vec3 &colour);

	int getIndex(int i, int j, int k, int m, int n) { return k * m * n + j * m + i; };
};
	

MarchingCube::MarchingCube()
{
	generateMeshTable();
}


MarchingCube::~MarchingCube()
{
}

inline void MarchingCube::generateMeshTable()
{
	int* currentInt;
	for (int i = 0; i < 256; i++) {

		currentInt = LUT.triTable[i];
		std::vector<float> currentMesh = {};

		for (int k = 0; k < 16; k+=3) {
			if(currentInt[k] != -1){
				generateTriangle(currentMesh, currentInt, k);
			}
			else {
				break;
			}
		}

		lookupMesh.push_back(currentMesh);
	}
}

inline std::vector<float> MarchingCube::generateMesh(float* booleanField, int m, int n, int l)
{
	std::vector<float> mesh = {};

	for (int k = 0; k < l - 1; k++) {
		for (int j = 0; j < n - 1; j++) {
			for (int i = 0; i < m - 1; i++) {
				int lookUpindex = 0;

				lookUpindex |= booleanField[getIndex(i, j + 1, k + 1, m, n)] ? 1 << 0 : 0 << 0;
				lookUpindex |= booleanField[getIndex(i + 1, j + 1, k + 1, m, n)] ? 1 << 1 : 0 << 1;
				lookUpindex |= booleanField[getIndex(i + 1, j, k + 1, m, n)] ? 1 << 2 : 0 << 2;
				lookUpindex |= booleanField[getIndex(i, j, k + 1, m, n)] ? 1 << 3 : 0 << 3;

				lookUpindex |= booleanField[getIndex(i, j + 1, k, m, n)] ? 1 << 4 : 0 << 4;
				lookUpindex |= booleanField[getIndex(i + 1, j + 1, k, m, n)] ? 1 << 5 : 0 << 5;
				lookUpindex |= booleanField[getIndex(i + 1, j, k, m, n)] ? 1 << 6 : 0 << 6;
				lookUpindex |= booleanField[getIndex(i,     j,     k, m, n)] ? 1 << 7 : 0 << 7;

				std::cout << "Logging lookUpIndex: " << lookUpindex << "\n";

				for (int x = 0; x < lookupMesh[lookUpindex].size(); x += 9) {

					mesh.push_back(lookupMesh[lookUpindex].data()[x] + i);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 1] - k);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 2] - j);

					mesh.push_back(lookupMesh[lookUpindex].data()[x + 3]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 4]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 5]);

					mesh.push_back(lookupMesh[lookUpindex].data()[x + 6]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 7]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 8]);
				}
			}
		}
	}

	return mesh;
}

inline void MarchingCube::generateTriangle(std::vector<float> &currentMesh, int* currentInt, int k)
{
	glm::vec3 v1 = LUT.vertices[currentInt[k]];
	glm::vec3 v2 = LUT.vertices[currentInt[k + 1]];
	glm::vec3 v3 = LUT.vertices[currentInt[k + 2]];

	glm::vec3 n = glm::normalize(glm::cross(v2 - v1, v3 - v1));
	glm::vec3 colour = glm::vec3(1.0f, 0.2f, 0.3f);

	// coordinates
	generateVertex(currentMesh, v1, n, colour);
	generateVertex(currentMesh, v2, n, colour);
	generateVertex(currentMesh, v3, n, colour);
}

inline void MarchingCube::generateVertex(std::vector<float> & currentMesh, glm::vec3 &v, glm::vec3 &n, glm::vec3 &colour)
{
	currentMesh.push_back(v.x);
	currentMesh.push_back(v.y);
	currentMesh.push_back(v.z);

	// normals
	currentMesh.push_back(n.x);
	currentMesh.push_back(n.y);
	currentMesh.push_back(n.z);

	// colours
	currentMesh.push_back(colour.x);
	currentMesh.push_back(colour.y);
	currentMesh.push_back(colour.z);
}
