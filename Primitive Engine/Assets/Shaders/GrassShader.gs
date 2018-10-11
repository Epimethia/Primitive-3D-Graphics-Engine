#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 72) out;

out vec3 outColor;
in VS_GS_VERTEX{
    in vec4 position;
    in vec3 color;
    in mat4 mvp;
} gs_in[];

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    outColor = gs_in[0].color;

    //Generating grass blades
    for (int i = -1; i < 2; i++) {
        vec4 pos = gs_in[0].position;
        pos.x += i * 0.03f;
        pos.z -= 0.7f;
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.018f, 0.0f); EmitVertex();
        EndPrimitive();
    }

    for (int i = -1; i < 2; i++) {
        vec4 pos = gs_in[0].position;
        pos.x += i * 0.05f;
        pos.y -= 0.05f;
        pos.z -= 0.7f;
    gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.018f, 0.0f); EmitVertex();
        EndPrimitive();
    }

        for (int i = -1; i < 2; i++) {
        vec4 pos = gs_in[0].position;
        pos.x += i * 0.05f;
        pos.y += 0.05f;
        pos.z -= 0.7f;     
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.0f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.006f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(-0.002f, 0.0f, 0.012f, 0.0f); EmitVertex();
        gl_Position = pos + gs_in[0].mvp * vec4(0.0f, 0.0f, 0.018f, 0.0f); EmitVertex();
        EndPrimitive();
    }
}