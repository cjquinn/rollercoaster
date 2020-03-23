#include "camera.h"

#include "canvas.h"
#include "cart.h"
#include "frame.h"
#include "window.h"

// Constructor for camera -- initialise with some default values
Camera::Camera() :
  position_(glm::vec3(352.065948f, 110.0f, 177.996780f)), view_(glm::vec3(349.362854f, 155.424336f, -183.370804f)),
  up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)), speed_(0.025f), state_(FREE_VIEW)
{
}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void Camera::set(glm::vec3 &position, glm::vec3 &view, glm::vec3 &up_vector)
{
  position_ = position;
  view_ = view;
  up_vector_ = up_vector;
}

// Respond to mouse movement
void Camera::setViewByMouse()
{
  glfwSetInputMode(Window::instance().glwfWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  int windowWidth, windowHeight;
  glfwGetFramebufferSize(Window::instance().glwfWindow(), &windowWidth, &windowHeight);

  int middle_x = windowWidth / 2;
  int middle_y = windowHeight / 2;

  float angle_y = 0.0f;
  float angle_z = 0.0f;
  static float rotation_x = 0.0f;

  double mouseX, mouseY;
  glfwGetCursorPos(Window::instance().glwfWindow(), &mouseX, &mouseY);

  if (int(mouseX) == middle_x && int(mouseY) == middle_y) {
    return;
  }

  glfwSetCursorPos(Window::instance().glwfWindow(), middle_x, middle_y);

  angle_y = (float) (middle_x - mouseX) / 1000.0f;
  angle_z = (float) (middle_y - mouseY) / 1000.0f;

  rotation_x -= angle_z;

  float max_angle = (float) M_PI / 2;
  if (rotation_x > max_angle) {
    rotation_x = max_angle;
  } else if (rotation_x < -max_angle) {
    rotation_x = -max_angle;
  } else {
    glm::vec3 cross = glm::cross(view_ - position_, up_vector_);
    glm::vec3 axis = glm::normalize(cross);

    rotateViewPoint(angle_z, axis);
  }

  glm::vec3 point = glm::vec3(0, 1, 0);
  rotateViewPoint(angle_y, point);
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void Camera::rotateViewPoint(float angle, glm::vec3 &point)
{
  glm::vec3 view = view_ - position_;
  
  glm::mat4 rotated = glm::rotate(glm::mat4(1), angle * 180.0f / (float) M_PI, point);
  glm::vec4 new_view = rotated * glm::vec4(view, 1);

  view_ = position_ + glm::vec3(new_view);
}

// Strafe the camera (side to side motion)
void Camera::strafe(double direction)
{
  float speed = (float) (speed_ * direction);

  position_.x = position_.x + strafe_vector_.x * speed;
  position_.z = position_.z + strafe_vector_.z * speed;

  view_.x = view_.x + strafe_vector_.x * speed;
  view_.z = view_.z + strafe_vector_.z * speed;
}

// Advance the camera (forward / backward motion)
void Camera::advance(double direction)
{
  float speed = (float) (speed_ * direction);

  glm::vec3 view = glm::normalize(view_ - position_);
  position_ = position_ + view * speed;
  view_ = view_ + view * speed;
}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void Camera::update(double dt)
{
	if (state_ == FREE_VIEW) {
		setViewByMouse();
		translateByKeyboard(dt);
		
		glm::vec3 cross = glm::cross(view_ - position_, up_vector_);
		strafe_vector_ = glm::normalize(cross);
	} else {
		if (state_ == BILLBOARD) {
			position_ = glm::vec3(352.065948f, 110.0f, 177.996780f);
			view_ = glm::vec3(349.362854f, 155.424336f, -183.370804f);
		}
		else if (state_ == TOP_VIEW) {
			position_ = glm::vec3(46.9f, 809.5f, 7.6f);
			view_ = glm::vec3(43.24f, 12.749f, 79.489f);
		} else {
			static float t = 0.0f;
			t += 0.005f * (float) dt;

			Frame *frame = Canvas::instance().cart()->frame();

			if (state_ == FIRST_PERSON) {
				position_ = frame->p() + 5.0f * frame->b() - frame->t() * 0.5f;
				view_ = frame->p() + 30.0f * frame->t();
			} else {
				position_ = frame->p() + frame->t() * 2.5f + frame->b() * 10.0f + frame->n() * 10.0f;
				view_ = frame->p() + frame->t() * 2.5f;
			}
		}
	}
}

// Update the camera to respond to key presses for translation
void Camera::translateByKeyboard(double dt)
{
  if (glfwGetKey(Window::instance().glwfWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    advance(3.0 * dt);
  }

  if (glfwGetKey(Window::instance().glwfWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    advance(-3.0 * dt);
  }

  if (glfwGetKey(Window::instance().glwfWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    strafe(-3.0 * dt);
  }

  if (glfwGetKey(Window::instance().glwfWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    strafe(3.0 * dt);
  }
}

// Return the camera position
glm::vec3 Camera::position() const
{
  return position_;
}

// Return the camera view point
glm::vec3 Camera::view() const
{
  return view_;
}

// Return the camera up vector
glm::vec3 Camera::up_vector() const
{
  return up_vector_;
}

// Return the camera strafe vector
glm::vec3 Camera::strafe_vector() const
{
  return strafe_vector_;
}

// Return the camera perspective projection matrix
glm::mat4 *Camera::perspective()
{
  return &perspective_;
}

// Return the camera orthographic projection matrix
glm::mat4 *Camera::orthographic()
{
  return &orthographic_;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void Camera::setPerspective(float fov, float ratio, float close, float end)
{
	perspective_ = glm::perspective(fov, ratio, close, end);
}

// The the camera orthographic projection matrix to match the width and height passed in
void Camera::setOrthographic(int width, int height)
{
  orthographic_ = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
}

// Get the camera view matrix
glm::mat4 Camera::view_matrix()
{
  return glm::lookAt(position_, view_, up_vector_);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 Camera::normal(const glm::mat4 &modelview)
{
  return glm::transpose(glm::inverse(glm::mat3(modelview)));
}

void Camera::setState(State state)
{
	state_ = state;
}

State Camera::state()
{
	return state_;
}