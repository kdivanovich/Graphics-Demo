#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;
uniform bool normalShader;
in vec3 worldPosition;

in float fIntensity;
out vec4 fragColour;

void main()
{
		// 
		if (fIntensity > 0.75)
			fragColour = vec4(0.7, 0.0, 0.7, 1.0);  //alpha is 0.5!!!
		else if (fIntensity > 0.5)
			fragColour = vec4(0.6, 0.0, 6.0, 0.0);
		else
			fragColour = vec4(0.3, 0.0, 0.3, 1.0);
	
}
