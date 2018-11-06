#version 450 core
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;
in vec4 mWorldPos;

out vec4 color;
uniform sampler2D tex;

uniform float ambientStr = 0.0f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(0.0f, 0.0f, -1000.0f);
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos;
uniform float shininess = 1.0f;

void main() {

    /*---------------------------------------*/
    /*|            PHONG SHADER             |*/
    /*---------------------------------------*/

    //ambient color
    vec3 ambient = ambientStr * ambientColor;
    // light direction
    vec3 norm = normalize(fragNormal);
    vec3 lightdir = normalize(fragPos - lightPos);
    // diffuse coloring
    float diffusestr = max(dot(norm, -lightdir ), 0.0f);
    vec3 diffuse = diffusestr * lightColor;

    // specular highlight
    vec3 negviewdir = normalize(camPos - fragPos);
    vec3 reflectdir = reflect(lightdir, norm);
    float spec = pow(max(dot(negviewdir , reflectdir), 0.0f), shininess);
    vec3 specular = lightSpecStr * spec * lightColor;

    color = texture(tex, fragTexCoord);//vec4(ambient + diffuse + specular, 1.0f) * texture(tex, fragTexCoord);

    // float d = distance(mWorldPos.xyz, camPos);
    // float lerp = (d - 1.0f)/10.0f;
    // lerp = clamp(lerp, 0.0, 1.0);
    // vec4 vfogcolor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    // color = mix(color, vfogcolor, lerp);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
}

