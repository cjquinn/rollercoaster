#version 330

// Structure for matrices
uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelViewMatrix; 
	mat3 normalMatrix;
} matrices;

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1; 
uniform MaterialInfo material1; 

// Layout of vertex attributes in VBO
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

// Vertex colour output to fragment shader -- using Gouraud (interpolated) shading
out vec4 colour;	// Colour computed using reflectance model
out vec2 texCoord;	// Texture coordinate

vec4 calculate_colour(vec4 light_dir, vec3 normal)
{
	vec4 colour;

	float intensity = dot(light_dir,normal);

	if (intensity > 0.95)
		colour = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		colour = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		colour = vec4(0.4,0.2,0.2,1.0);
	else
		colour = vec4(0.2,0.1,0.1,1.0);

	return colour;
}

// This is the entry point into the vertex shader
void main()
{	
	// Transform the vertex spatial position using 
	gl_Position = matrices.projMatrix * matrices.modelViewMatrix * vec4(inPosition, 1.0f);
	
	// Get the vertex normal and vertex position in eye coordinates
	vec3 vEyeNorm = normalize(matrices.normalMatrix * inNormal);
	vec4 vEyePosition = matrices.modelViewMatrix * vec4(inPosition, 1.0f);
		
	// Apply the Phong model to compute the vertex colour
	colour = calculate_colour(light1.position, inNormal);
	
	// Pass through the texture coordinate
	texCoord = inCoord;
}