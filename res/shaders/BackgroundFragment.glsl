#version 330 core

in vec2 pos;
out vec4 FragColor;

uniform float time;
uniform vec3 lightDir;
uniform mat4 cameraProjView;

void main()
{
    // todo, most of this is ChatGPT authored, want to go in and implement my own atmospheric
    // simulation for pretty sunsets

    vec3 skyHorizonColor = vec3(0.6, 0.7, 1.0);
    vec3 skyZenithColor  = vec3(0.1, 0.4, 1.0);
    vec3 twilightColor   = vec3(0.2, 0.1, 0.4)/2;  // Purplish dusk
    float atmosphereStrength = 1.0;
    float twilightStrength = 1.0;                // Helps control how strong twilight is
    
    
    // NDC to clipspace to worldspace
    vec4 clipPos  = vec4(pos, 1.0, 1.0); // z = 1 for the far plane
    vec4 worldPos = inverse(cameraProjView) * clipPos;
    worldPos     /= worldPos.w;
    vec3 rayDir   = normalize(worldPos.xyz);

    const vec3 UP_DIR = vec3(0.0, 1.0, 0.0);

    // 3) Basic Rayleigh: Blend between horizon and zenith
    float cosAngle   = dot(rayDir, UP_DIR);
    cosAngle         = cosAngle * 0.5 + 0.5;       // Range [-1..1] -> [0..1]
    cosAngle         = clamp(cosAngle, 0.0, 1.0);

    vec3 skyRayleigh = mix(skyHorizonColor, skyZenithColor, cosAngle);

    // 4) Approximate Mie: simple forward-scattering around the sun
    float cosViewSun = dot(rayDir, -lightDir);     // -lightDir since it's "from sun"
    float miePhase   = 1.0 / pow(1.0 + 0.05*(1.0 - cosViewSun*cosViewSun), 1.5);
    vec3  skyMie     = vec3(miePhase) * 0.02;

    // 5) Combine base sky colors
    vec3 color = skyRayleigh + skyMie;

    // 6) Small bright disc for the sun itself
    float sunFactor = max(0.0, cosViewSun);
    color += vec3(pow(sunFactor, 300.0));
    if (sunFactor > 0.999)
        color = vec3(1.0 + time);

    // 7) Twilight adjustment
    //    - sunAltitude > 0.0 => sun above horizon
    //    - sunAltitude < 0.0 => sun below horizon
    float sunAltitude = dot(-lightDir, UP_DIR);   // Dot with up => +1 overhead, 0 horizon, -1 below
    float h           = clamp(sunAltitude * 0.5 + 0.5, 0.0, 1.0); 
    // h=1 => sun overhead, h=0 => sun well below horizon

    // Blend toward dusk color as sunAltitude goes below horizon
    // (if sun is above horizon, this blend is minimal)
    vec3 twilightBlend = mix(twilightColor, color, h);
    color = mix(color, twilightBlend, twilightStrength);

    // 8) Globally scale with atmosphereStrength
    color *= atmosphereStrength;

    FragColor = vec4(color, 1.0);
}
