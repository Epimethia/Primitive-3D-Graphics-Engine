#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 13) out;

out vec3 outColor;
in VS_GS_VERTEX{
    in vec4 position;
    in vec3 color;
    in mat4 mvp;
} gs_in[];

void main() {
    outColor = vec3(1.0f, 1.0f, 1.0f);
    float r = 3.0f;
    float rad = (3.1415f / 180.0f);

    //point 0;
    float x = r * sin(0);
    float y = r * cos(0);
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //Point 7
    x = (r / 2.7f) * sin((7 * (36.0f * rad)));
    y = (r / 2.7f) * cos((7 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 4
    x = r * sin((4 * (36.0f * rad)));
    y = r * cos((4 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 0;
    x = r * sin(0);
    y = r * cos(0);
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //#####################

    //point 9
    x = (r / 2.7f) * sin((9 * (36.0f * rad)));
    y = (r / 2.7f) * cos((9 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //Point 6
    x = r * sin((6 * (36.0 * rad)));
    y = r * cos((6 * (36.0 * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 2
    x = r * sin((2 * (36.0f * rad)));
    y = r * cos((2 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 9
    x = (r / 2.7f) * sin((9 * (36.0f * rad)));
    y = (r / 2.7f) * cos((9 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //#####################

    //point 9
    x = (r / 2.7f) * sin((5 * (36.0f * rad)));
    y = (r / 2.7f) * cos((5 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //Point 6
    x = r * sin((2 * (36.0f * rad)));
    y = r * cos((2 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 2
    x = r * sin((8 * (36.0f * rad)));
    y = r * cos((8 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //point 9
    x = (r / 2.7f) * sin((5 * (36.0f * rad)));
    y = (r / 2.7f) * cos((5 * (36.0f * rad)));
    gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    EndPrimitive();

    //float x = r * sin(0);
    //float y = r * cos(0);
    //gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(x, y + 2.0f, 0.0f, 0.0f); EmitVertex();

    //gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-2.0f, 0.0f, 0.0f, 0.0f); EmitVertex();
    //gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(2.0f, 0.0f, 0.0f, 0.0f); EmitVertex();
    //gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(0.0f, 2.0f, 0.0f, 0.0f); EmitVertex();

}