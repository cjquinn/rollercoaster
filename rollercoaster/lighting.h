#ifndef LIGHTING_H
#define LIGHTING_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class ShaderProgram;

class Lighting
{
public:
	static void white();	
	static void setPosition(glm::vec4 position);
	static void setSpotlight(glm::vec4 position, glm::vec3 direction, float exponent, float cutoff);
	static void setSpotlightIntensity(float intensity);
	static void setSpotlightIntensity(glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity);
	static void setIntensity(float intensity);
	static void setIntensity(glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity);
	static void setReflectance(glm::vec3 ambient_reflectance, glm::vec3 diffuse_reflectance, glm::vec3 specular_reflectance, float shininess);
};

#endif