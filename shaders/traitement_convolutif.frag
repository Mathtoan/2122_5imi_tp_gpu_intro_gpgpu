#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
    color = vec4(0,0,0,0);
    for(int i=-3; i<3; i++){
        for(int j=-3; j<3; j++){
            color += (1./49)*texture(myTextureSampler,uv 
                                    + float(i)*vec2(0.005,0)
                                    + float(j)*vec2(0,0.005));
        }
    }
}
