#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 rainbowColor;
uniform vec3 helicopterColor;
uniform bool useRainbowColor;
uniform bool is_cube;

void main()
{
    // TODO: Implement Rainbow Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. Set FragColor to be the dot product of the color and rainbowColor
	//   Note: Ensure FragColor is appropriately set for both rainbow and normal cases.

	if(!is_cube){
		vec4 texcolor = texture(ourTexture, TexCoord);
		if(useRainbowColor){
			float r = texcolor.r * rainbowColor.r;
			float g = texcolor.g * rainbowColor.g;
			float b = texcolor.b * rainbowColor.b;
			FragColor = vec4(r, g, b, texcolor.a);
		}
		else{
			FragColor = texcolor;
		}
	}
	else{
		vec3 lightPos = vec3(0,200,100);
		vec3 lightColor = vec3(1,1,1);
		
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.5);
		vec3 diffuse = diff * lightColor;
			
		vec3 result = diffuse * helicopterColor;
		if(useRainbowColor){
			float r = result.r * rainbowColor.r;
			float g = result.g * rainbowColor.g;
			float b = result.b * rainbowColor.b;
			FragColor = vec4(r, g, b, 1.0);
		}
		else{
			FragColor = vec4(result, 1.0);
		}
	}
}