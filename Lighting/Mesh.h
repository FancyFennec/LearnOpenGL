#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void createMesh(float* vertices, float* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void renderMesh();
	void clearMesh();

private:
	unsigned int VAO, VBO, IBO;
	unsigned indexCount;
};

#endif