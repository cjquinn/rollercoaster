#version 330

in vec4 colour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 texCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 outputColour;		// The output colour

uniform sampler2D sampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied

void main()
{
	vec4 texColour = texture2D(sampler, texCoord);	

	if (bUseTexture)
		outputColour = texColour*colour;	// Combine object colour and texture 
	else
		outputColour = colour;	// Just use the colour instead	
}
