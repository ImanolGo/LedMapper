#version 150     // <-- version my machine suports
#extension GL_ARB_texture_rectangle : enable  // <-- not supported by my machine
#extension GL_EXT_gpu_shader4 : enable    // <-- not supported by my machine

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}