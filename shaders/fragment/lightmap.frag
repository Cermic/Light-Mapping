// Phong fragment shader phong-tex.frag matched with phong-tex.vert
#version 330

// Some drivers require the following
precision highp float;

struct lightStruct
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	
	float attConst;
	float attLinear;
	float attQuadratic;
};

struct materialStruct
{						
// We take ambient out because ambient doesnt affect the lighting much so we use the diffuse to calculate 
// lighting for the texture.

	vec4 diffuse;
	vec4 specular;
	float shininess;

};

uniform lightStruct light;
uniform materialStruct material;
uniform sampler2D textureUnit0;
uniform sampler2D textureUnit1;
uniform int textureVisible;
uniform int specularValue;

uniform sampler2D ambient;
uniform sampler2D diffuse;
uniform sampler2D speuclar;

in vec3 ex_N;
in vec3 ex_V;
in vec3 ex_L;
in vec2 ex_TexCoord;
in float ex_D;
layout(location = 0) out vec4 out_Color;	// shouldn't this be location = 1?
 
void main(void) {

	// Ambient 
	vec4 ambient = light.ambient ;

	// Diffuse 
	float diffuseI = max(dot(normalize(ex_N),normalize(ex_L)),0);
	vec4 diffuse = light.diffuse * diffuseI  * texture(textureUnit1, ex_TexCoord).r;  
	// red component = brightness or intensity of the diffuse
	// The diffuse intensity is given by the r(red) value of textureUnit1 at the relevant coordindate for the pixel.

	// Specular 
	// Calculate R - reflection of light
	vec3 R = normalize(reflect(normalize(-ex_L),normalize(ex_N)));

	// texture g(green) component is shininess
	float f_texel1_spec_shininess = pow(max(dot(R,ex_V),0), texture(textureUnit1, ex_TexCoord).g); 
	// The shininess is given by the g(green) value of textureUnit1 at the relevant coordindate for the pixel.
	// This is how the shader can create a variance in shininess. It's measuring the green value per pixel, not over the whole texture.
	
	vec4 specular =  light.specular * (specularValue * f_texel1_spec_shininess) * texture(textureUnit1, ex_TexCoord).b ; 
	// We are now using the texturecoords to decide the material specular value.
	// blue component of the texture is the specular intensity
	// The speuclar intensity is given by the b(blue) value of textureUnit1 at the relevant coordindate for the pixel.

	// Attenuation //
	float attenuation = light.attConst + light.attLinear * ex_D + light.attQuadratic * ex_D * ex_D;
	vec4 tmp_Color = ((diffuse) + (specular)); // Can change the specular here to exagerate the effect.
	//attenuation does not affect ambient light
	vec4 litColour = ambient+vec4(tmp_Color.rgb / attenuation, 1.0);

	// Fragment colour //
	
	// The actual material seen by the user should be the box which is done here.
	vec4 material_tex;
	if(textureVisible == 0)
	{
		material_tex = texture(textureUnit0, ex_TexCoord);
	}
	if(textureVisible == 1)
	{
		material_tex = texture(textureUnit1, ex_TexCoord);
	}
	out_Color = litColour * material_tex;
	// The colour parameters collected by our rgb values are now applied to the box texture to 
	// Create the appropriate lighting effects on the seperate parts of the box.

}