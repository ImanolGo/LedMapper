// fragment shader
 
#version 150

out vec4 outputColor;
uniform vec4 globalColor;
 
void main()
{
	outputColor = globalColor;
}