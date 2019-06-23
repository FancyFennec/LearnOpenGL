#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "stb_image.h"

class Model
{
public:
	Model(Mesh& mesh, Shader& shader);
	Model(Shader& shader);
	~Model();

	void renderModel() { mesh.RenderMesh(); };
	void useCubeMesh();
	void updateColourShader();

	Mesh& mesh;
	Shader& shader;

	float shininess;
};

#endif