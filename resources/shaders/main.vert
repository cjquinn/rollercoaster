#version 330

// Struct for matrices
uniform struct Matrices
{
	mat4 projection;
	mat4 modelview;
	mat3 normal;
} matrices;

// Layout of vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord_in;
layout (location = 2) in vec3 normal;

// Outputs for fragment shader
out vec3 eye_normal;
out vec4 eye_position;
out vec2 texture_coord;

void main()
{
	eye_normal = normalize(matrices.normal * normal);
	eye_position = matrices.modelview * vec4(position, 1.0f); 
	gl_Position = matrices.projection * eye_position;
	texture_coord = texture_coord_in;
}
