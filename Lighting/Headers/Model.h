#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"


class Model
{
public:
	Model(Mesh mesh, Shader shader);
	~Model();

	void loadTextures(const char* diffuse, const char* specular);
	unsigned int loadTexture(char const * path);

private:
	Mesh mesh;
	Shader shader;

	unsigned int diffuseMap;
	unsigned int specularMap;
};

