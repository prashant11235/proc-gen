#version 440 core

struct Light
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Different levels of height for texture mapping.
const float water = 0.2;
const float grass = 0.7;


vec3 blendColor(vec3 rgbA, vec3 rgbB, vec3 rgbC, float alpha);

in vec3 FragPos;
in vec3 Color;
in vec3 Normal;
in vec2 texCoords;

out vec4 FragColor; 

uniform sampler2D stoneTex;
uniform sampler2D grassTex;
uniform sampler2D waterTex;

uniform vec3 viewPos;
uniform Light light;

float rand(vec2 co){
    float a = fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);	
	return a;
}

vec3 texture_mapping(vec3 position, vec2 texCoords)
{
    vec3 mapped;

    if (position.y < water) {
        mapped = texture(waterTex, texCoords).rgb;
    } else if (position.y < grass) {
        mapped = texture(grassTex, texCoords).rgb;
    } else {
        mapped = texture(stoneTex, texCoords).rgb;
    }
    return mapped;
}

void main()
{
    // texture
    
    //vec3 grass = texture(grassTex, texCoords).rgb;
    vec3 material = texture_mapping(FragPos, texCoords);

    vec3 result = material; 
    FragColor = vec4(result, 1.0f);
}

vec3 blendColor(vec3 rgbA, vec3 rgbB, vec3 rgbC, float alpha)
{
	return (rgbA * alpha) + (rgbB * alpha) + (rgbC * alpha);
}

