// Fragment shader:
// ================
#version 330 core

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Colour;
in vec3 Normal;

uniform Light light;
uniform mat4 view;
uniform mat4 lsmodel;

uniform float shininess;

void main()
{

	// ambient
	vec3 ambient  = light.ambient * Colour;
   
     // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view * lsmodel * vec4(light.position,1.0)) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * diff * Colour;
    
    // specular
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * Colour;  
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

