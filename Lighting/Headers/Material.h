#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Material
{
public:
	Material();
	Material(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Material();

	void setDiffuse(float diffuse) { this->diffuse = glm::vec3(diffuse); };
	void setSpecular(float specular) { this->specular = glm::vec3(specular); };

	void setDiffuse(glm::vec3 diffuse) { this->diffuse = diffuse; };
	void setSpecular(glm::vec3 specular) { this->specular = specular; };

	glm::vec3 diffuse;
	glm::vec3 specular;

	float shininess;
};

#endif