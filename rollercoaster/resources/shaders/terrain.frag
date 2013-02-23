#version 330

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
in float intensity;
out vec4 vOutputColour;

void main()
{	
	if(intensity < 0.5) {
		//dark
		vOutputColour = vec4(0.235f, 0.352f, 0.592f, 1.0f);  
	}
	else if(intensity < 0.75) {
		//medium
		vOutputColour = vec4(0.333f, 0.431f, 0.65f, 1.0f);
	}else{
		//light
		vOutputColour = vec4(0.529f, 0.662f, 0.772f, 1.0f);
	}
}
