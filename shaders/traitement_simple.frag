#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
    color = texture( myTextureSampler, uv );
    float moy = (color.x + color.y + color.z ) / 3;

    //NOIR ET BLANC
    //color.x = moy;
    //color.y = moy;
    //color.z = moy;

    //ROUGE ET BLEU
    color.y = 0.f;

    //ROUGE ET VERT
    //color.z = 0.f;

    //VERT ET BLEU
    //color.x = 0.f;
}
