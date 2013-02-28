#include "lighting.h"

#include "canvas.h"
#include "matrixstack.h"
#include "shaderprogram.h"

void Lighting::white()
{
	Camera *camera = Canvas::instance().camera();
	ShaderProgram *main = (Canvas::instance().shader_programs())[0];
	main->use();

	main->setUniform("light1.position", glm::vec3(0.0f, 0.0f, 0.0f)); // Position of light source in eye coordinates
  main->setUniform("light1.La", glm::vec3(1.0f)); // Ambient colour of light
  main->setUniform("light1.Ld", glm::vec3(1.0f)); // Diffuse colour of light
  main->setUniform("light1.Ls", glm::vec3(1.0f)); // Specular colour of light
  main->setUniform("material1.Ma", glm::vec3(1.0f)); // Ambient material reflectance
  main->setUniform("material1.Md", glm::vec3(0.0f)); // Diffuse material reflectance
  main->setUniform("material1.Ms", glm::vec3(0.0f)); // Specular material reflectance
  main->setUniform("material1.shininess", 15.0f); // Shininess material property
}

void Lighting::set(
		int shader_program,
		glm::vec4 position,
		glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity,
		glm::vec3 ambient_reflectance, glm::vec3 diffuse_reflectance, glm::vec3 specular_reflectance, 
		float shininess) {
	
	ShaderProgram *main = (Canvas::instance().shader_programs())[shader_program];
	main->use();

	main->setUniform("light1.position", position); // Position of light source in eye coordinates
  main->setUniform("light1.La", ambient_intensity); // Ambient colour of light
  main->setUniform("light1.Ld", diffuse_intensity); // Diffuse colour of light
  main->setUniform("light1.Ls", specular_intensity); // Specular colour of light
  main->setUniform("material1.Ma", ambient_reflectance); // Ambient material reflectance
  main->setUniform("material1.Md", diffuse_reflectance); // Diffuse material reflectance
  main->setUniform("material1.Ms", specular_reflectance); // Specular material reflectance
  main->setUniform("material1.shininess", shininess); // Shininess material property
}