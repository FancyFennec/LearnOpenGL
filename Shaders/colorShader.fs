// Fragment shader:
// ================
#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform mat4 view;
uniform mat4 lsmodel;

void main()
{

	// ambient
	vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
   
     // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view * lsmodel * vec4(light.position,1.0)) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // specular
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

