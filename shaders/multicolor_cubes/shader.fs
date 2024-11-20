#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord),
    //                 texture(texture2, TexCoord), opacity);
    FragColor = vec4(color.x, color.y, color.z, 1.0);
    // FragColor = vec4(0.0, 1.0, 1, 1.0);
}
