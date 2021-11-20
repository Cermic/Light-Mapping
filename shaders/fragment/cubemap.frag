// cubeMap fragment shader
#version 330

precision highp float;

out vec4 outColour;
in vec3 cubeTexCoord;
uniform samplerCube cubeMap;
 
void main(void) {   
	outColour = texture(cubeMap, cubeTexCoord);
}
