#version 330

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in float intensity;
in float edge;
out vec4 vOutputColour;

void main()
{	
	int levels = 3;

	vOutputColour.rgb = floor(vColour.rgb * levels) / levels;
}
