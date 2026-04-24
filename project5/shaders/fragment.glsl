#version 330

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float damageFlash; // 0.0 = no flash, 1.0 = peak flash

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
    vec4 color = texture(texture0, fragTexCoord) * fragColor * colDiffuse;

    // Player red damage flash
    vec3 flashedColor = mix(color.rgb, vec3(0.35, 0.0, 0.0), damageFlash * 0.1);

    finalColor = vec4(flashedColor, color.a);
}
