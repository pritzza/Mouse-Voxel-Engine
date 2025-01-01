#version 330 core

in vec2 pos;

out vec4 FragColor;

uniform float time;
uniform vec3 lightDir;
uniform mat4 cameraProjView;

void main()
{
    // chat authored (literally just asked for a "cool" fragment shader):

    // Base UV coordinates from pos
    vec2 uv = pos;

    // Add time-varying distortion
    float t = time * 0.5;
    uv.x += 0.1 * sin(10.0 * uv.y + t);
    uv.y += 0.1 * cos(10.0 * uv.x - t);

    // Generate color channels with sine waves
    vec3 color;
    color.r = 0.5 + 0.5 * sin((uv.x + uv.y) * 6.0 + t);
    color.g = 0.5 + 0.5 * sin(uv.y * 12.0 - 2.0 * t);
    color.b = 0.5 + 0.5 * sin(uv.x * 12.0 + 2.0 * t);


    // sun

    // NDC to clipspace to worldspace
    vec4 clipPos = vec4(pos, 1.0, 1.0); // z = 1 for far plane
    vec4 worldPos = inverse(cameraProjView) * clipPos;
    worldPos /= worldPos.w; // Perspective divide to get 3D position

    vec3 rayDir = normalize(worldPos.xyz);

    float lighting = max(0.0, dot(rayDir, -lightDir));
    
    color += pow(lighting, 100.f);
    
    if (lighting > 0.999)
        color = vec3(1.0);

    FragColor = vec4(color, 1.0);
}
