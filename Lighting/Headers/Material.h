#pragma once

#include <glm/glm.hpp>

class Material
{
public:
	Material();
	~Material();

	void setAmbient(float ambient) { this->ambient = glm::vec3(ambient); };
	void setDiffuse(float diffuse) { this->diffuse = glm::vec3(diffuse); };
	void setSpecular(float specular) { this->specular = glm::vec3(specular); };

	void setAmbient(glm::vec3 ambient) { this->ambient = ambient; };
	void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; };
	void setSpecular(glm::vec3 specular) { this->specular = specular; };

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};



Material::Material()
{
	ambient = glm::vec3(0);
	diffuse = glm::vec3(0);
	specular = glm::vec3(0);
	shininess = 0.f;
}


Material::~Material()
{
}
