#version 450 core
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;
in vec4 mWorldPos;

out vec4 color;
uniform sampler2D tex;

uniform float ambientStr = 0.6f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(1000.0f, 1000.0f, 1000.0f);
uniform float lightSpecStr = 0.8f;
uniform vec3 camPos;
uniform float shininess = 2.0f;

void main() {

    /*---------------------------------------*/
    /*|          BLINN PHONG SHADER         |*/
    /*---------------------------------------*/

    //AMBIENT COLOR
    vec3 ambient = ambientStr * ambientColor;
    // Light Direction
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragPos - lightPos);
    // Diffuse Coloring
    float diffuseStr = max(dot(norm, -lightDir ), 0.0f);
    vec3 diffuse = diffuseStr * lightColor;

    // Specular Highlight
    vec3 negViewDir = normalize(camPos - fragPos);
    vec3 halfwayVec = normalize(-lightDir + negViewDir);
    float spec = pow(max(dot(norm , halfwayVec), 0.0f), shininess);
    vec3 specular = lightSpecStr * spec * lightColor;

    color = vec4(ambient + diffuse + specular, 1.0f) * texture(tex, fragTexCoord);

    float d = distance(mWorldPos.xyz, camPos);
    float lerp = (d - 1.0f)/10.0f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

    color = mix(color, vFogColor, lerp);
    
}

