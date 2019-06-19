#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"
#include "Material.h"


class Model
{
public:
	Model(Mesh mesh, Shader shader, Material material);
	~Model();

	void loadTextures(const char* diffuse, const char* specular);
	unsigned int loadTexture(char const * path);

	void renderModel();

private:
	Mesh mesh;
	Shader shader;

	Material material;

	unsigned int diffuseMap;
	unsigned int specularMap;
};

