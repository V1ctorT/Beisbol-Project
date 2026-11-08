#version 330 core
in float alpha;
out vec4 color;
uniform vec3 beamColor;
uniform float opacidad;
void main()
{
    color = vec4(beamColor, alpha * opacidad);
}