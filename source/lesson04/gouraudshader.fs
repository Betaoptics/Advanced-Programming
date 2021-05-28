uniform sampler2D texture01;
uniform vec4 materialAmbient;
uniform vec4 materialEmissive;
varying vec2 outUv;
varying vec4 outColor;

void main(void)
{
    gl_FragColor = materialAmbient +
        (outColor * texture2D(texture01, outUv)) +
        materialEmissive;
}

