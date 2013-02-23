#include "lighting.h"

#include "camera.h"
#include "canvas.h"
#include "matrixstack.h"
#include "shaderprogram.h"

void Lighting::set(
		glm::vec4 position,
		glm::vec3 ambient_intensity, glm::vec3 diffuse_intensity, glm::vec3 specular_intensity,
		glm::vec3 ambient_reflectance, glm::vec3 diffuse_reflectance, glm::vec3 specular_reflectance, 
		float shininess) {

	Camera *camera = Canvas::instance().camera();
	ShaderProgram *main = (Canvas::instance().shader_programs())[0];

	main->setUniform("light1.position", position); // Position of light source in eye coordinates
  main->setUniform("light1.La", ambient_intensity); // Ambient colour of light
  main->setUniform("light1.Ld", diffuse_intensity); // Diffuse colour of light
  main->setUniform("light1.Ls", specular_intensity); // Specular colour of light
  main->setUniform("material1.Ma", ambient_reflectance); // Ambient material reflectance
  main->setUniform("material1.Md", diffuse_reflectance); // Diffuse material reflectance
  main->setUniform("material1.Ms", specular_reflectance); // Specular material reflectance
  main->setUniform("material1.shininess", shininess); // Shininess material property
}