// textured.frag
#version 330

// Some drivers require the following
precision highp float;

uniform sampler2D textureUnit0;

in vec2 ex_TexCoord;
layout(location = 0) out vec4 out_Color;
 
void main(void) {
   vec4 texelsCulled =	texture(textureUnit0, ex_TexCoord);

	if(texelsCulled.a < 0.5)
    discard;

	// Fragment colour
	out_Color = texelsCulled;
}