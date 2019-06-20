#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "stb_image.h"



class Model
{
public:
	Model(Mesh *mesh, Shader* shader, Material material);
	Model(Mesh *mesh, Shader* shader, glm::vec3 colour);
	~Model();

	void loadTextures(const char* diffuse, const char* specular);
	unsigned int loadTexture(char const * path);

	void bindMaps();
	void renderModel() { mesh->RenderMesh(); };
	void updateShader();
	void updateColourShader();

	Mesh *mesh;
	Shader* shader;
	Material material;

	glm::vec3 colour;
	float shininess;

private:
	unsigned int diffuseMap;
	unsigned int specularMap;
};

#endif