#version 330 core


uniform vec4 aColor;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color; // 전달받은 컬러 값

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine lighting with object color
    vec3 result = (ambient + diffuse + specular) * Color;
    FragColor = vec4(result, 1.0);
	if(aColor.w > 0){
		FragColor = aColor;
	}
}
