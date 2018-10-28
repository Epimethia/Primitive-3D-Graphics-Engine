#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

out vec3 outColor;
out vec2 fragTexCoord;

in VS_GS_VERTEX{
    in vec4 position;
    in vec3 color;
    in mat4 mvp;
} gs_in[];

/*Found from stack exchange somewhere*/
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {

    outColor = gs_in[0].color;
    vec4 pos = gs_in[0].position;

    //First Quad
    gl_Position = pos + gs_in[0].mvp * vec4(0.02f, 0.0f, 0.0f, 0.0f);
    fragTexCoord = vec2(1.0f, 1.0f);
    EmitVertex();

    //Bottom Right
    gl_Position = pos + gs_in[0].mvp * vec4(-0.02f, 0.0f, 0.0f, 0.0f);
    fragTexCoord = vec2(0.0f, 1.0f);
    EmitVertex();
    
    //Top left
    gl_Position = pos + gs_in[0].mvp * vec4(0.02f, 0.0f, -0.02f, 0.0f);
    fragTexCoord = vec2(1.0f, 0.0f);
    EmitVertex();

    //Bottom Left
    gl_Position = pos + gs_in[0].mvp * vec4(-0.02f, 0.0f, -0.02f, 0.0f);
    fragTexCoord = vec2(0.0f, 0.0f);
    EmitVertex();
    EndPrimitive();


    //Second Quad
    mat4 mpvrotated = rotationMatrix(vec3(0.0f, 0.0f, 1.0f), 90.0f);
    gl_Position = pos + mpvrotated * vec4(0.02f, 0.0f, 0.0f, 0.0f);
    fragTexCoord = vec2(1.0f, 1.0f);
    EmitVertex();

    //Bottom Right
    gl_Position = pos + mpvrotated * vec4(-0.02f, 0.0f, 0.0f, 0.0f);
    fragTexCoord = vec2(0.0f, 1.0f);
    EmitVertex();
    
    //Top left
    gl_Position = pos + mpvrotated * vec4(0.02f, 0.0f, -0.02f, 0.0f);
    fragTexCoord = vec2(1.0f, 0.0f);
    EmitVertex();

    //Bottom Left
    gl_Position = pos + mpvrotated * vec4(-0.02f, 0.0f, -0.02f, 0.0f);
    fragTexCoord = vec2(0.0f, 0.0f);
    EmitVertex();
    EndPrimitive();
   
}