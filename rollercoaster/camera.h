#ifndef CAMERA_H
#define CAMERA_H

#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

class Spline;

class Camera 
{
public:
  // Constructor - sets default values for camera position, viewvector, upvector, and speed
  Camera();

  // Gets the position of the camera centre of projection
  glm::vec3 position() const;

  // Gets the position of the camera view point
  glm::vec3 view() const;

  // Gets the camera up vector
  glm::vec3 up_vector() const;

  // Gets the camera strafe vector
  glm::vec3 strafe_vector() const;

  // Gets the camera perspective projection matrix
  glm::mat4 *perspective();

  // Gets the camera orthographic projection matrix
  glm::mat4 *orthographic();

  // Gets the camera view matrix - note this is not stored in the class but returned using glm::lookAt() in viewMatrix()
  glm::mat4 view_matrix();

  // Set the camera position, viewpoint, and up vector
  void set(glm::vec3 &position, glm::vec3 &view, glm::vec3 &up_vector);
  
  // Rotate the camera viewpoint -- this effectively rotates the camera
  void rotateViewPoint(float theta, glm::vec3 &point);

  // Respond to mouse movement to rotate the camera
  void setViewByMouse();

  // Respond to keyboard presses on arrow keys to translate the camera
  void translateByKeyboard(double dt);

  // Strafe the camera (move it side to side)
  void strafe(double direction);

  // Advance the camera (move it forward or backward)
  void advance(double direction);

  // Update the camera
  void update(double dt);

  // Set the projection matrices
  void setPerspective(float fov, float aspect_ratio, float near, float far);
  void setOrthographic(int width, int height);

  glm::mat3 normal(const glm::mat4 &modelview);

	// Follow spline
	void follow(Spline *spline);

private:
  // The position of the camera's centre of projection
  glm::vec3 position_;

  // The camera's viewpoint (point where the camera is looking)
  glm::vec3 view_;

  // The camera's up vector
  glm::vec3 up_vector_;

  // The camera's strafe vector
  glm::vec3 strafe_vector_;

  // How fast the camera moves
  float speed_;

  // Perspective projection matrix
  glm::mat4 perspective_;

  // Orthographic projection matrix
  glm::mat4 orthographic_;

	Spline *spline_;
};

#endif
