#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(float *vertices, unsigned int numOfVertices);
	void CreateCubeMesh();
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	unsigned int VAO, VBO;
	unsigned int numOfVertices;
};

#endif