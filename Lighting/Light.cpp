#include "Headers/Light.h"


Light::Light(Shader* shader) : shader(shader)
{
	ambient = glm::vec3(0);
	diffuse = glm::vec3(0);
	specular = glm::vec3(0);
	position = glm::vec3(0);
}

Light::Light(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	shader(shader),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
	position = glm::vec3(0);
}

Light::Light(Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position) :
	shader(shader),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	position(position) {}


Light::~Light()
{
}

void Light::updateShader(glm::vec3& viewPos)
{
	shader->setVec3("light.ambient", ambient); // note that all light colors are set at full intensity
	shader->setVec3("light.diffuse", diffuse);
	shader->setVec3("light.specular", specular);
	shader->setVec3("light.position", position);
	shader->setVec3("viewPos", viewPos);
}