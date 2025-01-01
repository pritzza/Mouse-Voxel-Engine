#version 330 core

in vec2 pos;

out vec4 FragColor;

uniform float time;

uniform vec3 viewDir;
uniform vec3 lightDir;

uniform mat4 cameraProj;

// chat authored
void main()
{
    // Project the combined direction vector and extract an x-component offset
    vec4 dirProj = cameraProj * vec4(viewDir + lightDir, 1.0);
    float offset = dirProj.x;

    // Base UV coordinates from pos
    vec2 uv = pos;

    // Add time-varying distortion
    float t = time * 0.5;
    uv.x += 0.1 * sin(10.0 * uv.y + t + offset);
    uv.y += 0.1 * cos(10.0 * uv.x - t - offset);

    // Generate color channels with sine waves
    vec3 color;
    color.r = 0.5 + 0.5 * sin((uv.x + uv.y) * 6.0 + t);
    color.g = 0.5 + 0.5 * sin(uv.y * 12.0 - 2.0 * t + offset);
    color.b = 0.5 + 0.5 * sin(uv.x * 12.0 + 2.0 * t - offset);

    FragColor = vec4(color, 1.0);
}
