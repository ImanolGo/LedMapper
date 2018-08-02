#version 150

uniform sampler2DRect tex0;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);

     if(texel0.r < 0.1 ){
        texel0.a = texel0.r ;
    }

    outputColor = texel0;
}