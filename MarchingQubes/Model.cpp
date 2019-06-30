#include "Headers/Model.h"



Model::Model(Mesh& mesh, Shader& shader) :
	shader(shader),
	mesh(mesh)
{
	shader.use();
	shininess = 32;
}

Model::~Model()
{
}

void Model::updateColourShader()
{
	shader.setFloat("shininess", shininess);
}
