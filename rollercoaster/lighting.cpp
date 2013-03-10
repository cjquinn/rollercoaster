#include "lighting.h"

#include "canvas.h"
#include "matrixstack.h"
#include "shaderprogram.h"

void Lighting::white()
{
	Camera *camera = Canvas::instance().camera();
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();

	main->setUniform("light.position", glm::vec3(0.0f, 0.0f, 0.0f)); // Position of light source in eye coordinates
  main->setUniform("light.ambient", glm::vec3(1.0f)); // Ambient colour of light
  main->setUniform("light.diffuse", glm::vec3(1.0f)); // Diffuse colour of light
  main->setUniform("light.specular", glm::vec3(1.0f)); // Specular colour of light
  main->setUniform("material.ambient", glm::vec3(1.0f)); // Ambient material reflectance
  main->setUniform("material.diffuse", glm::vec3(0.0f)); // Diffuse material reflectance
  main->setUniform("material.specular", glm::vec3(0.0f)); // Specular material reflectance
  main->setUniform("material.shininess", 15.0f); // Shininess material property
}

void Lighting::set(
		glm::vec4 position,
		glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity,
		glm::vec3 ambient_reflectance, glm::vec3 diffuse_reflectance, glm::vec3 specular_reflectance, 
		float shininess) {
	
	ShaderProgram *main = Canvas::instance().shader_programs();
	main->use();

	main->setUniform("light.position", position); // Position of light source in eye coordinates
  main->setUniform("light.ambient", ambient_intensity); // Ambient colour of light
  main->setUniform("light.diffuse", diffuse_intensity); // Diffuse colour of light
  main->setUniform("light.specular", specular_intensity); // Specular colour of light
  main->setUniform("material.ambient", ambient_reflectance); // Ambient material reflectance
  main->setUniform("material.diffuse", diffuse_reflectance); // Diffuse material reflectance
  main->setUniform("material.specular", specular_reflectance); // Specular material reflectance
  main->setUniform("material.shininess", shininess); // Shininess material property
}