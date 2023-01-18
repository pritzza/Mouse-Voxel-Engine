#version 330 core

in vec4 ourColor;
in vec2 textureCoordinate;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float time;

void main()
{
    FragColor = mix(
                    texture(texture0, textureCoordinate), 
                    texture(texture1, textureCoordinate), 
                    (sin(time)/2.0) + 0.5
                );
} 