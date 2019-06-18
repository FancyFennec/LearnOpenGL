#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

class Mesh
{
public:
	Mesh(unsigned int numOfVertices);

	void CreateMesh(float *vertices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	unsigned int VAO, VBO;
	unsigned int numOfVertices;
};

#endif