#version 140
out vec4 fragColorOut;


#include "sdf.glsl"

uniform sampler2D texUnit;
uniform float modulation;
uniform vec4 box;
uniform vec4 cornerRadius;

in vec2 uv;
in vec2 vertex;

void main(void)
{
    vec4 fragColor = texture(texUnit, uv);

    float f = sdfRoundedBox(vertex, box.xy, box.zw, cornerRadius);
    float df = fwidth(f);
    float alpha = clamp(0.5 - f / df, 0.0, 1.0);

    fragColorOut = vec4(fragColor.rgb, modulation * alpha);
}
