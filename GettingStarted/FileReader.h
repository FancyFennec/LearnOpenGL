#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class mesh
{
public:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	Shader shader = Shader("testShader.vs", "testShader.fs");

	unsigned int VBO, VAO, EBO;

	mesh(const char filePath[]) {

		FILE *file = fopen(filePath, "r");

		// Check if the file is NULL
		if (file == NULL) {
			printf("File not found.");
		} else {
			//Loop through all the lines
			while (true) {
				char lineHeader[128];
				int res = fscanf(file, "%s", lineHeader);
				if (res == EOF)
					break;

				//Get all verteces
				if (strcmp(lineHeader, "v") == 0) {
					float v[3];
					fscanf(file, "%f %f %f\n", &v[0], &v[1], &v[2]);
					for (float vert : v) {
						vertices.push_back(vert);
					}
				}

				//Get all normal vectors
				else if (strcmp(lineHeader, "vn") == 0) {
					float n[3];
					fscanf(file, "%f %f %f\n", &n[0], &n[1], &n[2]);
					for (float norm : n) {
						normals.push_back(norm);
					}
				}

				//Create Triangles from normals and verteces
				else if (strcmp(lineHeader, "f") == 0) {
					unsigned int vIndex[3], nIndex;

					int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vIndex[0], &nIndex, &vIndex[1], &nIndex, &vIndex[2], &nIndex);
					if (matches != 6) {
						printf("file can't be read by our simple parser : ( try exporting with other options\n");

					}
					for (unsigned int ind : vIndex) {
						indices.push_back(ind -1);
					}
					normals.push_back(nIndex - 1);
				}
			}

			printf("Successfully loaded triangles\n");
			std::cout << vertices.size() << "\n";
			std::cout << indices.size() << "\n";
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// 4. then set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		shader.use();
	}

	void draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	~mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};

#endif