#pragma once

#include "Tables.h"
#include "Headers/pNoise.h"

class MarchingCube
{
public:
	MarchingCube();
	MarchingCube(const char *filename);
	~MarchingCube();

	std::vector<float> generateMRIMesh(int x, int y, int z);
	std::vector<float> generatePerlinMesh(int x, int y, int z);
	std::vector<float> generateMesh(std::vector<float> &field, int m, int n, int k);
	std::vector<float> updateIsoLevel(float& isoLevel);
	std::vector<float> updateIsoLevel(float& isoLevel, float& isoLevelTop);

	std::vector<std::vector<float>> lookupMesh;

private:
	int initialiseData(const char* filename, std::vector<float> &values);
	void generateMeshTable();
	void generateTriangle(std::vector<float> &currentMesh, int* currentInt, int k);
	void generateVertex(std::vector<float> & currentMesh, glm::vec3 &v, glm::vec3 &n, glm::vec3 &colour);

	int getIndex(int i, int j, int k, int m, int n) { return k * m * n + j * m + i; };

	std::vector<float> valueField;
	int m; 
	int n; 
	int l;
};
	

MarchingCube::MarchingCube()
{
	generateMeshTable();
}

// TODO: This does not yet work together
inline MarchingCube::MarchingCube(const char* filename)
{
	valueField = {};

	generateMeshTable();
	if (!initialiseData(filename, valueField)) {
		std::cout << "ERRROR!!! Could not load file :(" << std::endl;
	}
	else {
		std::cout << "Vector size: " << valueField.size() << std::endl;
	}
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

inline std::vector<float> MarchingCube::generateMRIMesh(int x, int y, int z)
{
	std::vector<float> gridValues = {};

#pragma omp parallel for
	for (int i = -1; i < x + 1; ++i) {
		for (int j = -1; j < y + 1; ++j) {
			for (int k = -1; k < z + 1; ++k) {
				if (i == -1 || j == -1 || k == -1 || i == x || j == y || k == z) {
					gridValues.push_back(0);
				}
				else {
					gridValues.push_back(valueField[getIndex(i, j, k, x, y)]);
				}

			}
		}
	}

	return  generateMesh(gridValues, x + 2, y + 2, z + 2);
}

inline std::vector<float> MarchingCube::generatePerlinMesh(int x, int y, int z)
{
	PerlinNoise pNoise = PerlinNoise(4);
	std::vector<float> gridValues = {};

#pragma omp parallel for
	for (int i = -1; i < x + 1; ++i) {
		for (int j = -1; j < y + 1; ++j) {
			for (int k = -1; k < z + 1; ++k) {
				if (i == -1 || j == -1 || k == -1 || i == x || j == y || k == z) {
					gridValues.push_back(0);
				}
				else {
					gridValues.push_back((float)pNoise.noise(
						3 * (double)i / ((double)x),
						3 * (double)j / ((double)y),
						3 * (double)k / ((double)z)));
				}

			}
		}
	}

	return  generateMesh(gridValues, x + 2, y + 2, z + 2);
}

inline std::vector<float> MarchingCube::generateMesh(std::vector<float> &field, int m, int n, int l)
{
	valueField = field;
	this->m = m;
	this->n = n;
	this->l = l;

	float isoLevel = 60.0f;
	std::vector<float> mesh = {};

#pragma omp parallel for
	for (int k = 0; k < l - 1; k++) {
		for (int j = 0; j < n - 1; j++) {
			for (int i = 0; i < m - 1; i++) {
				int lookUpindex = 0;

				lookUpindex |= valueField[getIndex(i    , j + 1, k + 1, m, n)] > isoLevel ? 1 << 0 : 0 << 0;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k + 1, m, n)] > isoLevel ? 1 << 1 : 0 << 1;
				lookUpindex |= valueField[getIndex(i + 1, j    , k + 1, m, n)] > isoLevel ? 1 << 2 : 0 << 2;
				lookUpindex |= valueField[getIndex(i    , j    , k + 1, m, n)] > isoLevel ? 1 << 3 : 0 << 3;

				lookUpindex |= valueField[getIndex(i    , j + 1, k, m, n)] > isoLevel ? 1 << 4 : 0 << 4;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k, m, n)] > isoLevel ? 1 << 5 : 0 << 5;
				lookUpindex |= valueField[getIndex(i + 1, j    , k, m, n)] > isoLevel ? 1 << 6 : 0 << 6;
				lookUpindex |= valueField[getIndex(i    , j    , k, m, n)] > isoLevel ? 1 << 7 : 0 << 7;

				for (int x = 0; x < lookupMesh[lookUpindex].size(); x += 9) {

					mesh.push_back(lookupMesh[lookUpindex].data()[x] + i);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 1] - k);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 2] - j);

					mesh.push_back(lookupMesh[lookUpindex].data()[x + 3]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 4]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 5]);

					mesh.push_back(
						(float)k /float(l) *lookupMesh[lookUpindex].data()[x + 6]
						+ (1- (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 7]
						+ (1 - (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 8]
						+ (1 - (float)k / float(l)));
				}
			}
		}
	}

	return mesh;
}

inline std::vector<float> MarchingCube::updateIsoLevel(float& isoLevel)
{
	std::vector<float> mesh = {};

#pragma omp parallel for
	for (int k = 0; k < l - 1; k++) {
		for (int j = 0; j < n - 1; j++) {
			for (int i = 0; i < m - 1; i++) {
				int lookUpindex = 0;

				lookUpindex |= valueField[getIndex(i, j + 1, k + 1, m, n)] > isoLevel ? 1 << 0 : 0 << 0;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k + 1, m, n)] > isoLevel ? 1 << 1 : 0 << 1;
				lookUpindex |= valueField[getIndex(i + 1, j, k + 1, m, n)] > isoLevel ? 1 << 2 : 0 << 2;
				lookUpindex |= valueField[getIndex(i, j, k + 1, m, n)] > isoLevel ? 1 << 3 : 0 << 3;

				lookUpindex |= valueField[getIndex(i, j + 1, k, m, n)] > isoLevel ? 1 << 4 : 0 << 4;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k, m, n)] > isoLevel ? 1 << 5 : 0 << 5;
				lookUpindex |= valueField[getIndex(i + 1, j, k, m, n)] > isoLevel ? 1 << 6 : 0 << 6;
				lookUpindex |= valueField[getIndex(i, j, k, m, n)] > isoLevel ? 1 << 7 : 0 << 7;

				for (int x = 0; x < lookupMesh[lookUpindex].size(); x += 9) {

					mesh.push_back(lookupMesh[lookUpindex].data()[x] + i);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 1] - k);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 2] - j);

					mesh.push_back(lookupMesh[lookUpindex].data()[x + 3]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 4]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 5]);

					mesh.push_back(
						(float)k / float(l) *lookupMesh[lookUpindex].data()[x + 6]
						+ (1 - (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 7]
						+ (1 - (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 8]
						+ (1 - (float)k / float(l)));
				}
			}
		}
	}

	return mesh;
}

inline std::vector<float> MarchingCube::updateIsoLevel(float& isoLevel, float& isoLevelTop)
{
	std::vector<float> mesh = {};

#pragma omp parallel for
	for (int k = 0; k < l - 1; k++) {
		for (int j = 0; j < n - 1; j++) {
			for (int i = 0; i < m - 1; i++) {
				int lookUpindex = 0;

				lookUpindex |= valueField[getIndex(i, j + 1, k + 1, m, n)] > isoLevel &&
					valueField[getIndex(i, j + 1, k + 1, m, n)] <= isoLevelTop
					? 1 << 0 : 0 << 0;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k + 1, m, n)] > isoLevel &&
					valueField[getIndex(i + 1, j + 1, k + 1, m, n)] <= isoLevelTop
					? 1 << 1 : 0 << 1;
				lookUpindex |= valueField[getIndex(i + 1, j, k + 1, m, n)] > isoLevel &&
					valueField[getIndex(i + 1, j, k + 1, m, n)] <= isoLevelTop
					? 1 << 2 : 0 << 2;
				lookUpindex |= valueField[getIndex(i, j, k + 1, m, n)] > isoLevel &&
					valueField[getIndex(i, j, k + 1, m, n)] <=isoLevelTop
					? 1 << 3 : 0 << 3;

				lookUpindex |= valueField[getIndex(i, j + 1, k, m, n)] > isoLevel &&
					valueField[getIndex(i, j + 1, k, m, n)] <= isoLevelTop
					? 1 << 4 : 0 << 4;
				lookUpindex |= valueField[getIndex(i + 1, j + 1, k, m, n)] > isoLevel &&
					valueField[getIndex(i + 1, j + 1, k, m, n)] <= isoLevelTop
					? 1 << 5 : 0 << 5;
				lookUpindex |= valueField[getIndex(i + 1, j, k, m, n)] > isoLevel &&
					valueField[getIndex(i + 1, j, k, m, n)] <= isoLevelTop
					? 1 << 6 : 0 << 6;
				lookUpindex |= valueField[getIndex(i, j, k, m, n)] > isoLevel &&
					valueField[getIndex(i, j, k, m, n)]  <= isoLevelTop
					? 1 << 7 : 0 << 7;

				for (int x = 0; x < lookupMesh[lookUpindex].size(); x += 9) {

					mesh.push_back(lookupMesh[lookUpindex].data()[x] + i);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 1] - k);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 2] - j);

					mesh.push_back(lookupMesh[lookUpindex].data()[x + 3]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 4]);
					mesh.push_back(lookupMesh[lookUpindex].data()[x + 5]);

					mesh.push_back(
						(float)k / float(l) *lookupMesh[lookUpindex].data()[x + 6]
						+ (1 - (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 7]
						+ (1 - (float)k / float(l)));
					mesh.push_back((float)k / float(l) *lookupMesh[lookUpindex].data()[x + 8]
						+ (1 - (float)k / float(l)));
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

inline int MarchingCube::initialiseData(const char* filename, std::vector<float> &values)
{
	std::ifstream myfile(filename);

	if (!myfile) {
		std::cout << "ERRROR!!! Could not load file :(" << std::endl;
		return 0;
	}
	
	auto getValue = [](char c) {return (float)c - 48; };
	auto isNumeric = [](int x) {return (x >= 0 && x < 10); };

	std::string line;
	bool wasNumber = false;
	int  x = 0;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			for (char a : line) {
				if (wasNumber) {
					int newX = getValue(a);

					if (isNumeric(newX)) {
						x = x * 10 + newX;
					}
					else {
						values.push_back((float)x);
						wasNumber = false;
					}
				}
				else {
					x = getValue(a);
					if (isNumeric(x)) {
						wasNumber = true;
					}
				}
			}

		}
		myfile.close();
	}

	return 1;
}