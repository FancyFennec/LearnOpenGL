#include "Headers/Mesh.h"

Mesh::Mesh() 
{
	VAO = 0;
	VBO = 0;
	numOfVertices = 0;
}

void Mesh::CreateMesh(float *vertices, unsigned int numOfVertices)
{
	ClearMesh();

	this->numOfVertices = numOfVertices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 9 * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 9, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 9, (void*)(sizeof(vertices[0]) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::UpdateMesh(float * vertices, unsigned int numOfVertices)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * 9 * numOfVertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CreateCubeMesh()
{
	float vertices[] = {
		// positions          // normals           // colour
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.2f, 0.3f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.2f, 0.3f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.2f, 0.3f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.2f, 0.3f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.2f, 0.3f
	};

	CreateMesh(vertices, 36);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

Mesh::~Mesh()
{
	ClearMesh();
}
