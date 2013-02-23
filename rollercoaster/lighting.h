#ifndef LIGHTING_H
#define LIGHTING_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class ShaderProgram;

class Lighting
{
public:
	static void set(
		int shader_program,
		glm::vec4 position,
		glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity,
		glm::vec3 ambient_reflectance, glm::vec3 diffuse_reflectance, glm::vec3 specular_reflectance, 
		float shininess);
};

#endif