#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

class Light
{
public:
	Light(Shader& shader);
	Light(Shader& shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	Light(Shader& shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position);
	~Light();

	void updateShader(glm::vec3& viewPos);
	void updateShader();

	void setAmbient(float& ambient) { this->ambient = glm::vec3(ambient); };
	void setDiffuse(float& diffuse) { this->diffuse = glm::vec3(diffuse); };
	void setSpecular(float& specular) { this->specular = glm::vec3(specular); };
	void setViewPos(float& viewPos) { this->viewPos = glm::vec3(viewPos); };

	void setAmbient(glm::vec3& ambient) { this->ambient = ambient; };
	void setDiffuse(glm::vec3& diffuse) { this->diffuse = diffuse; };
	void setSpecular(glm::vec3& specular) { this->specular = specular; };
	void setViewPos(glm::vec3& viewPos) { this->viewPos = viewPos; };

	void setPosition(glm::vec3& position) { this->position = position; };

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;
	glm::vec3 viewPos;

	Shader& shader;
};

#endif