#include "Headers/Material.h"


Material::Material()
{
	diffuse = glm::vec3(0);
	specular = glm::vec3(0);

	shininess = 0.f;
}

Material::Material(glm::vec3 diffuse, glm::vec3 specular, float shininess) :
	diffuse(diffuse),
	specular(specular),

	shininess(shininess) {}

Material::~Material()
{
}