#version 330

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D gSampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

void main()
{
	// Get the texel colour from the texture sampler
	vec4 vTexColour = texture2D(gSampler, vTexCoord);	

	if (bUseTexture)
		vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
	else
		vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead	
}
