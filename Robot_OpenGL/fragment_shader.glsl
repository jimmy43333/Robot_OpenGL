#version 410 core

out vec4 fcolor;

uniform sampler2D tex_object;
uniform int notex;
in VS_OUT{
    vec2 tc;
}fs_in;

in vec4 color;

void main(void)
{
    if(notex == 1){
        fcolor = color;
    }else{
        fcolor = texture(tex_object, fs_in.tc*vec2(1,1));
    }
}

