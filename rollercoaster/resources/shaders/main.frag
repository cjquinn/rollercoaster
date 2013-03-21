#version 330

// Struct for light
uniform struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

uniform struct SpotLight
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
	float exponent;
	float cutoff;
} spotlight;

// Struct for material
uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

// Flags
uniform bool texture_fragment;
uniform bool toonify;

// Sampler
uniform sampler2D sampler;

// Inputs from vertex shader
in vec3 eye_normal;
in vec4 eye_position;
in vec2 texture_coord;

// Outputs from fragment shader
out vec4 fragment_colour;

vec4 blinnPhongSpotLight()
{
	vec3 ambient = spotlight.ambient * material.ambient;		

	vec3 s = normalize(spotlight.position.xyz - eye_position.xyz);	
	float angle = acos(dot(-s, spotlight.direction));
	float cutoff = radians(clamp(spotlight.cutoff, 0.0, 90.0));
	
	if (angle < cutoff) {
		vec3 v = normalize(-eye_position.xyz);
		vec3 h = normalize(v + s);
		vec3 n = eye_normal;
	
		float s_dot_n = max(dot(s, n), 0.0f);
		
		vec3 diffuse = spotlight.diffuse * material.diffuse * s_dot_n;
		vec3 specular = vec3(0.0f);
		
		if(s_dot_n > 0.0f) {
			float eps = 0.000001f;
			specular = spotlight.specular * material.specular * pow(max(dot(h, v), 0.0f), material.shininess + eps);
		}

		float spotfactor = pow(dot(-s, spotlight.direction), spotlight.exponent);
		return vec4(ambient + spotfactor * (diffuse + specular), 1.0f);
	} else {
		return vec4(0.0f);
	}
}

vec4 blinnPhongLight() {
	vec3 s = normalize(light.position.xyz - eye_position.xyz);
	vec3 v = normalize(-eye_position.xyz);
	vec3 h = normalize(v + s);
	vec3 n = eye_normal;
	
	float s_dot_n = max(dot(s, n), 0.0f);

	vec3 ambient = light.ambient * material.ambient;		
	vec3 diffuse = light.diffuse * material.diffuse * s_dot_n;
	vec3 specular = vec3(0.0f);

	if(s_dot_n > 0.0f) {
		float eps = 0.000001f;
		specular = light.specular * material.specular * pow(max(dot(h, v), 0.0f), material.shininess + eps);
	}
	
	return vec4(ambient + diffuse + specular, 1.0f);
}

void main()
{
	vec4 light_colour = blinnPhongLight() + blinnPhongSpotLight();
	
	if(texture_fragment) {
		vec4 texture_colour = texture(sampler, texture_coord) * light_colour;
		light_colour = texture_colour;
	}

	if(toonify) {
		vec3 v = normalize(-eye_position.xyz);
		// Find the edges
		float edge_mask = (dot(v, normalize(eye_normal)) > 0.1) ? 1 : 0;

		// Levels of colour
		int levels = 8;
		fragment_colour = vec4((floor(light_colour.rgb * levels) / levels) * edge_mask, light_colour.w);
	}else{
		fragment_colour = light_colour;
	}
}